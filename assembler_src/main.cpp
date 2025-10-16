#include "assembler.h"
#include "code_reader.h"
#include "debug.h"

#include <stdio.h>


int main(int argc, char* argv[])
{
    files_info files = {};
    assembler_info asm_data = {};
    debug_info debug = {};

    err_t parsed = parse_args(argc, argv, &files, &asm_data);
    if (parsed != ok)
    {
        return 0;
    }

    err_t initialised = initialise_assembler(argc, argv, &asm_data, &debug);
    if (initialised != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler:", "aborting due to error\n");
        return 0;
    }

    printf_gen_info(asm_data.debug_mode, MAKE_BOLD("+++ ASSEMBLER +++\n\n"), NULL); // ask
    printf_gen_info(asm_data.debug_mode, "version: %d\n", version);
    printf_gen_info(asm_data.debug_mode, "designed by LMD (c) \n\n", NULL); // va_args

    printf_log_msg(asm_data.debug_mode, "assembler: began assembly\n\n", NULL);
    printf_log_msg(asm_data.debug_mode, "assembler: preliminary compilation began\n", NULL);

    err_t processed = preliminary_process_code(&files, &asm_data, &debug);

    if (processed != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler:", "aborting due to error\n");
        return 0;
    }
    printf_log_msg(asm_data.debug_mode, "assembler: preliminary compilation finished\n", NULL);

    if (asm_data.debug_mode == on)
        output_labels(&asm_data);

    printf_log_msg(asm_data.debug_mode, "assembler: final compilation began\n", NULL);

    rewind(files.input_file);
    fill_file_preamble(&files, asm_data.pos);

    process_code(&files, &asm_data, &debug);

    if (processed != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler:", "aborting due to error\n"); // macros
        return 0;
    }

    printf_log_msg(asm_data.debug_mode, "assembler: final compilation finished\n", NULL);

    check_warnings(&debug, &files, &asm_data);

    free(asm_data.str);
    fclose(files.input_file);
    //printf_empty_line(debug_mode);
    printf_gen_info(asm_data.debug_mode, "assembler: finished assembly\n", NULL);
    return 0;
}
