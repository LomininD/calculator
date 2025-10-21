#include "code_reader.h"

void readline(assembler_info* asm_data, FILE* fp)
{
    assert(asm_data != NULL);
    assert(fp != NULL);

    size_t symbol_ind = 0;
    char c = 0;

    while (true)
    {
        c = fgetc(fp);

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


err_t read_arg(char* file_name, assembler_info* asm_data, int current_line, arg_t arg_type)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    err_t is_read = ok;

    switch(arg_type)
    {
        case none:
            break;
        case number:
            is_read = read_number_arg(file_name, asm_data, current_line);
            return is_read;
        case string:
            is_read = read_string_arg(file_name, asm_data, current_line);
            return is_read;
        case ind:
            is_read = read_index(file_name, asm_data, current_line);
            return is_read;
        case label:
            is_read = replace_label(file_name, asm_data, current_line);
            return is_read;
        default:
            return error;
    }
    return ok;
}


err_t read_number_arg(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    md_t debug_mode = asm_data->debug_mode;

    int number = 0;
    char bad_symb = 0;

    int success = sscanf(asm_data->str, "%*s %d %c", &number, &bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_number_arg: recognised number %d\n", number);
        asm_data->code[asm_data->pos + preamble_size] = number;
        printf_log_msg(debug_mode, "read_number_arg: code[%zu] = %d\n", asm_data->pos + preamble_size, number);
        asm_data->pos++;
        return ok;
    }
    else
    {
        success = sscanf(asm_data->str, "%*s :%d", &number);

        if (success == 1)
        {
            return replace_label(file_name, asm_data, current_line);
        }
        else
        {
            printf_err(debug_mode, "[%s:%d] -> read_number_arg: failed to get number\n", file_name, current_line);
            return error;
        }
    }
}


err_t read_string_arg(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    md_t debug_mode = asm_data->debug_mode;

    int number = 0;
    char arg[3] = {};
    char bad_symb = 0;

    int success = sscanf(asm_data->str, "%*s %2s %c", arg, &bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_string_arg: recognised register name %s\n", arg);

        number = decode_reg_name(arg);
        printf_log_msg(debug_mode, "read_string_arg: got register index: %d\n", number);

        asm_data->code[asm_data->pos + preamble_size] = number;
        printf_log_msg(debug_mode, "read_string_arg: code[%zu] = %d\n", asm_data->pos + preamble_size, number);
        asm_data->pos++;
        return ok;
    }
    else
    {
        printf_err(debug_mode, "[%s:%d] -> read_string_arg: failed to get register name\n", file_name, current_line);
        return error;
    }
}


err_t read_index(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    md_t debug_mode = asm_data->debug_mode;

    int number = 0;
    char arg[3] = {};
    char raw_arg[5] = {};
    char bad_symb = 0;

    int success = sscanf(asm_data->str, "%*s %4s %c", raw_arg, &bad_symb);

    if (success == 1)
    {
        if (raw_arg[0] == '[' && raw_arg[3] == ']')
        {
            sscanf(raw_arg, "[%2s]", arg);

            printf_log_msg(debug_mode, "read_index: recognised register name %s\n", arg);

            number = decode_reg_name(arg);
            printf_log_msg(debug_mode, "read_index: got register index: %d\n", number);

            asm_data->code[asm_data->pos + preamble_size] = number;
            printf_log_msg(debug_mode, "read_index: code[%zu] = %d\n", asm_data->pos + preamble_size, number);
            asm_data->pos++;
            return ok;
        }
    }

    printf_err(debug_mode, "[%s:%d] -> read_string_arg: failed to get register name\n", file_name, current_line);
    return error;
}


err_t read_label(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    md_t debug_mode = asm_data->debug_mode;

    int number = 0;
    char bad_symb = 0;

    int success = sscanf(asm_data->str, ":%d %c", &number, &bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "read_label: recognised label %d\n", number);

        if (number < 0 || number >= max_labels_number)
        {
            printf_err(debug_mode, "[%s:%d] -> read_label: wrong label number\n", file_name, current_line);
            return error;
        }

        asm_data->labels[number] = asm_data->pos;

        return ok;
    }
    else
    {
        printf_err(debug_mode, "[%s:%d] -> read_label: failed to get label\n", file_name, current_line);
        return error;
    }
}


err_t replace_label(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    md_t debug_mode = asm_data->debug_mode;

    int number = -1;
    char bad_symb = 0;

    int success = sscanf(asm_data->str, "%*s :%d %c", &number, &bad_symb);

    if (success == 1)
    {
        printf_log_msg(debug_mode, "replace_label: recognised label %d\n", number);

        if (number < 0 || number >= max_labels_number)
        {
            printf_err(debug_mode, "[%s:%d] -> replace_label: wrong label number\n", file_name, current_line);
            return error;
        }

        int address = asm_data->labels[number];

        asm_data->code[asm_data->pos + preamble_size] = address;
        printf_log_msg(debug_mode, "replace_label: code[%zu] = %d\n", asm_data->pos + preamble_size, address);
        asm_data->pos++;

        return ok;
    }
    else
    {
        printf_err(debug_mode, "[%s:%d] -> replace_label: failed to get label\n", file_name, current_line);
        return error;
    }
}


int decode_reg_name(char* arg)
{
    assert(arg != NULL);

    int reg = arg[0] - 'A';
    return reg;
}
