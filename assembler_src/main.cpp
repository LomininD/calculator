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

    fill_file_preamble(&files);

    err_t processed = process_code(&files, &asm_data, &debug);

    if (processed != ok)
    {
        printf("\n");
        printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
        return 0;
    }

    check_warnings(&debug, &files);

    free(asm_data.str);
    printf("assembler: finished assembly\n");
    fclose(files.input_file);
    fclose(files.output_file);
    return 0;
}
