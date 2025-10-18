# ifndef ARG_PARSER
# define ARG_PARSER

#include "assembler_properties.h"
#include <string.h>

struct parser_struct
{
    bool input_file_determined;
    bool output_file_determined;
    bool got_o_flag;
};

err_t parse_args(int argc, char* argv[], files_info* files, assembler_info* asm_data);
err_t parse_flags(char flag_str[], db_mode* debug_mode, parser_struct* verification);
err_t parse_file_name(files_info* files, char* files_str, parser_struct* verification);
char* generate_output_name(char* input_file_name);
void launch_help(void);
err_t verify_file_names(files_info* files, parser_struct* verification);
err_t open_files(files_info* files, parser_struct* verification);

#endif
