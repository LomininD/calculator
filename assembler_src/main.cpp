#include "assembler.h"
#include "code_reader.h"
#include "../shared/debug.h"
#include "arg_parser.h"

#include <stdio.h>

// TODO: listing

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

    err_t initialised = asm_ctor(&asm_data, &debug);
    if (initialised != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler\n");
        return 0;
    }

    printf(MAKE_BOLD("+++ ASSEMBLER +++\n\n"), NULL);
    printf("version: %d\n", version);
    printf("designed by LMD (c) \n\n", NULL);

    printf_log_msg(asm_data.debug_mode, "assembler: began assembly\n\n", NULL);
    printf_log_msg(asm_data.debug_mode, "assembler: preliminary compilation began\n", NULL);

    err_t processed = process_code(&files, &asm_data, &debug);
    if (processed != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler\n");
        return 0;
    }
    printf_log_msg(asm_data.debug_mode, "assembler: preliminary compilation finished\n", NULL);

    if (asm_data.debug_mode == on)
        output_labels(asm_data.labels, asm_data.debug_mode);

    printf_log_msg(asm_data.debug_mode, "assembler: final compilation began\n", NULL);

    fill_file_preamble(&asm_data);
    reset_data(&asm_data, &debug.current_line, files.input_file);

    process_code(&files, &asm_data, &debug);
    if (processed != ok)
    {
        printf_abortion(asm_data.debug_mode, "assembler\n");
        return 0;
    }

    printf_log_msg(asm_data.debug_mode, "assembler: final compilation finished\n", NULL);

    check_warnings(&debug, files.input_file_name, asm_data.debug_mode);

    output_code(files.output_file, asm_data.code, asm_data.pos, asm_data.debug_mode);

    asm_dtor(&files, &asm_data);

    printf("assembler: finished assembly\n", NULL);
    return 0;
}
