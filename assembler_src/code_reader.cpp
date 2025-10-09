#include "code_reader.h"

void readline(assembler_info* asm_data, files_info* file)
{
    assert(asm_data != NULL);
    assert(file != NULL);

    size_t symbol_ind = 0;
    char c = 0;

    while (true)
    {
        c = fgetc(file->input_file);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            if (c == EOF)
                asm_data->end = true;

            break;
        }

        if (c == ';')
            *(asm_data->str + symbol_ind) = '\0';
        else
            *(asm_data->str + symbol_ind) = toupper(c);

        symbol_ind++;

        if (symbol_ind == asm_data->len)
        {
            asm_data->len *= 2;
            asm_data->str = (char*) realloc(asm_data->str, (asm_data->len + 1) * sizeof(char));
        }
    }

    *(asm_data->str + symbol_ind) = '\0';
}


err_t read_number_arg(files_info* files, assembler_info* asm_data, debug_info* debug, int* number)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);
    assert(number != NULL);

    int success = sscanf(asm_data->str, "%*s %d", number);

    if (success == 1)
    {
        printf("read_number_arg: recognised arg %d\n", *number);
        fprintf(files->output_file, "%d\n", *number);
        return ok;
    }
    else
    {
        printf("\n");
        printf("read_number_arg: %s:%d: " MAKE_BOLD_RED("ERROR:") " failed to get arg\n", files->input_file_name, debug->current_line);
        return wrong_number;
    }
}


err_t read_string_arg(files_info* files, assembler_info* asm_data, debug_info* debug, int* number)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);
    assert(number != NULL);

    char arg[3] = {};
    int success = sscanf(asm_data->str, "%*s %2s", arg); // how to fix wrong symbols in the end

    if (success == 1)
    {
        printf("read_string_arg: recognised arg %s\n", arg);

        *number = decode_reg_name(arg);

        printf("read_string_arg: got register index: %d\n", *number);
        fprintf(files->output_file, "%d\n", *number);
        return ok;
    }
    else
    {
        printf("\n");
        printf("read_string_arg: %s:%d: " MAKE_BOLD_RED("ERROR:") " failed to get arg\n", files->input_file_name, debug->current_line);
        return wrong_number;
    }
}


int decode_reg_name(char* arg)
{
    assert(arg != NULL);

    int reg = arg[0] - 'A';
    return reg;
}
