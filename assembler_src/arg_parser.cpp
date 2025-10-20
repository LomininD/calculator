#include <assert.h>
#include "arg_parser.h"


err_t parse_args(int argc, char* argv[], files_info* files, assembler_info* asm_data)
{
    assert(argv != NULL);
    assert(files != NULL);
    assert(asm_data != NULL);

    parser_struct verification = {false, false, false};

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            err_t got_flags = parse_flags(argv[i], asm_data, &verification);
            if (got_flags != ok)
                return got_flags;
        }
        else
        {
            err_t got_files = parse_file_name(files, argv[i], &verification);
            if (got_files != ok)
                return got_files;
        }
    }

    err_t verified = verify_file_names(files, &verification);

    files->output_defined = verification.got_o_flag;

    if (verified)
        return open_files(files, &verification, asm_data);
    else
        return error;
}

err_t parse_flags(char flag_str[], assembler_info* asm_data, parser_struct* verification)
{
    assert(asm_data != NULL);
    assert(verification != NULL);

    int ind = 1;
    while(flag_str[ind] != '\0')
    {
        if (flag_str[ind] == 'd')
        {
            asm_data->debug_mode = on;
        }
        else if (flag_str[ind] == 'l') // TODO: does not developed yet
        {
            asm_data->listing_mode = on;
        }
        else if (flag_str[ind] == 'o')
        {
            verification->got_o_flag = true;
        }
        else if (flag_str[ind] == 'h')
        {
            launch_help();
            return help;
        }
        else
        {
            printf(MAKE_BOLD_RED("ERROR:") " wrong flag(s)\n");
            printf(MAKE_GREY("Note: for more info add -h flag.\n"));
            return error;
        }
        ind++;
    }
    return ok;
}


err_t parse_file_name(files_info* files, char* files_str, parser_struct* verification)
{
    assert(files != NULL);
    assert(verification != NULL);

    if (!verification->input_file_determined)
    {
        files->input_file_name = files_str;
        verification->input_file_determined = true;
    }
    else if (verification->got_o_flag)
    {
        files->output_file_name = files_str;
        verification->output_file_determined = true;
    }
    else
    {
        printf(MAKE_BOLD_RED("ERROR:") " wrong command line argument\n");
        printf(MAKE_GREY("Note: for more info add -h flag.\n"));
        return error;
    }
    return ok;
}


char* generate_output_name(char* input_file_name)
{
    assert(input_file_name != NULL);

    const char* out_ext = "out";

    char* output_file_name = (char*) calloc(strlen(input_file_name) + 1, sizeof(char));

    int i = 0;
    while(*(input_file_name + i) != '.')
    {
        *(output_file_name + i) = *(input_file_name + i);
        i++;
    }

    *(output_file_name + i) = *(input_file_name + i);
    i++;

    int j = 0;
    while(*(out_ext + j) != '\0')
    {
        *(output_file_name + i) = *(out_ext + j);
        i++;
        j++;
    }
    return output_file_name;
}

void launch_help(void)
{
    printf(MAKE_BOLD("===ASSEMBLER HELP===\n\n"));
    printf("Input file name is required for assembler.\n\n");
    printf("Possible flags:\n");
    printf("-d: debug mode (log stored in asm_log.log)\n");
    printf("-o: define output file name after flags\n");
    printf("-l: makes listing file (asm_listing.lst) (not finished yet)\n");
    printf("-h: prints help information\n\n");
    printf("If output file is undefined, the file with same name but .out extention will be created.\n");
}

err_t verify_file_names(files_info* files, parser_struct* verification)
{
    assert(files != NULL);
    assert(verification != NULL);

    if (!verification->input_file_determined || verification->got_o_flag && !verification->output_file_determined)
    {
        printf(MAKE_BOLD_RED("ERROR:") " required files were not provided.\n");
        printf(MAKE_GREY("Note: for more info add -h flag.\n"));
        return error;
    }
    return ok;
}

err_t open_files(files_info* files, parser_struct* verification, assembler_info* asm_data)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(verification != NULL);

    if (!verification->got_o_flag)
        files->output_file_name = generate_output_name(files->input_file_name);

    files->input_file = fopen(files->input_file_name, "r");
    files->output_file = fopen(files->output_file_name, "w");

    if (asm_data->debug_mode == on)
    {
        log_ptr = fopen("asm_log.log", "w");
    }

    if (asm_data->listing_mode == on)
    {
        listing_ptr = fopen("asm_listing.lst", "w");
    }

    if (files->input_file == NULL                               || \
        files->output_file == NULL                              || \
        (asm_data->debug_mode == on && log_ptr == NULL)         || \
        (asm_data->listing_mode == on && listing_ptr == NULL))
    {
        printf(MAKE_BOLD_RED("ERROR:") " a problem with opening files occurred\n");
        printf(MAKE_GREY("Note: file name may be incorrect\n"));
        return error;
    }



    return ok;
}
