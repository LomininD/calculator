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


err_t read_number_arg(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    int number = 0;

    int success = sscanf(asm_data->str, "%*s %d", &number);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_number_arg: recognised number %d\n", number);
        asm_data->code[asm_data->pos + preamble_size] = number;
        asm_data->pos++;
        return ok;
    }
    else
    {
        success = sscanf(asm_data->str, "%*s :%d", &number);

        if (success == 1)
        {
            return replace_label(files, asm_data, debug);
        }
        else
        {
            //printf_empty_line(debug_mode);
            printf_err(debug_mode, "[%s:%d] -> read_number_arg: failed to get number\n", files->input_file_name, debug->current_line);
            return error;
        }
    }
}


err_t read_string_arg(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    int number = 0;

    char arg[3] = {};
    int success = sscanf(asm_data->str, "%*s %2s", arg); // fix

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_string_arg: recognised arg %s\n", arg);

        number = decode_reg_name(arg);

        printf_log_msg(debug_mode, "read_string_arg: got register index: %d\n", number);
        asm_data->code[asm_data->pos + preamble_size] = number;
        asm_data->pos++;
        return ok;
    }
    else
    {
        //printf_empty_line(debug_mode);
        printf_err(debug_mode, "[%s:%d] -> read_string_arg: failed to get arg\n", files->input_file_name, debug->current_line);
        return error;
    }
}


err_t read_label(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    int number = 0;

    char bad_symb[2] = {};
    int success = 0;

    success = sscanf(asm_data->str, ":%d %1s", &number, bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_label: recognised label %d\n", number);

        if (number < 0 || number >= max_labels_number)
        {
            printf_err(debug_mode, "[%s:%d] -> read_label: wrong label number\n", files->input_file_name, debug->current_line);
            return error;
        }

        asm_data->labels[number] = asm_data->pos;

        return ok;
    }
    else
    {
        //printf_empty_line(debug_mode);
        printf_err(debug_mode, "[%s:%d] -> read_label: failed to get label\n", files->input_file_name, debug->current_line);
        return error;
    }
}



err_t replace_label(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    int number = -1;

    char bad_symb[2] = {};
    int success = 0;

    success = sscanf(asm_data->str, "%*s :%d %s", &number, bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "replace_label: recognised label %d\n", number);

        if (number < 0 || number >= max_labels_number)
        {
            printf_err(debug_mode, "[%s:%d] -> replace_label: wrong label number\n", files->input_file_name, debug->current_line);
            return error;
        }

        int address = asm_data->labels[number];

        asm_data->code[asm_data->pos + preamble_size] = address;
        asm_data->pos++;

        return ok;
    }
    else
    {
        //printf_empty_line(debug_mode);
        printf_err(debug_mode, "[%s:%d] -> replace_label: failed to get label\n", files->input_file_name, debug->current_line);
        return error;
    }
}


int decode_reg_name(char* arg)
{
    assert(arg != NULL);

    int reg = arg[0] - 'A';
    return reg;
}
