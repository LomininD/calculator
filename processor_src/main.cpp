#include "processor_cmd.h"
#include "processor.h"


int main(int argc, char* argv[])
{
    printf(MAKE_BOLD("+++ PROCESSOR +++\n\n"));

    FILE* fp = NULL;

    err_t opened = open_file(&fp, argc, argv);

    if (opened != ok)
    {
        printf("main: terminating process due to error\n");
        return 0;
    }

    proc_info proc = {};
    err_t initialised = proc_ctor(fp, &proc);

    if (initialised != ok)
        END_PROCESS;

    read_byte_code(fp, &proc);

    proc_commands current_cmd = UNKNOWN;

    while (proc.ip < proc.prg_size)
    {
        current_cmd = (proc_commands) proc.code[proc.ip];
        err_t executed = execute_cmd(&proc, current_cmd);

        if (executed != ok)
            END_PROCESS;

        if (current_cmd == HLT)
            break;

        getchar();
    }

    printf("main: shutting down processor\n");
    proc_dtor(fp, &proc);
    return 0;
}
