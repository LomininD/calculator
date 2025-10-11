#include "assembler.h"
#include "code_reader.h"

#include <stdio.h>


int main(int argc, char* argv[])
{
    printf(MAKE_BOLD("+++ ASSEMBLER +++\n\n"));
    printf("version: %d\n", version);
    printf("designed by LMD (c) \n\n");

    files_info files = {};
    assembler_info asm_data = {};
    debug_info debug = {};

    err_t initialised = initialise_assembler(argc, argv, &files, &asm_data, &debug);
    if (initialised != ok)
    {
        printf("\n");
        printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
        return 0;
    }

    printf("assembler: began assembly\n\n");

    printf("assembler: preliminary compilation began\n");
    err_t processed = preliminary_process_code(&files, &asm_data, &debug);

    if (processed != ok)
    {
        printf("\n");
        printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
        return 0;
    }
    printf("assembler: preliminary compilation finished\n\n");

    output_labels(&asm_data);

    printf("assembler: final compilation began\n");

    rewind(files.input_file);
    fill_file_preamble(&files, asm_data.pos);

    process_code(&files, &asm_data, &debug);

    if (processed != ok)
    {
        printf("\n");
        printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
        return 0;
    }

    printf("assembler: final compilation finished\n");

    check_warnings(&debug, &files);

    free(asm_data.str);
    fclose(files.input_file);
    printf("assembler: finished assembly\n");
    return 0;
}
