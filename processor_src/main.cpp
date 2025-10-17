#include "processor_cmd.h"
#include "processor.h"

// *1000 to work with floats 


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

    proc_info spu = {};
    err_t initialised = proc_ctor(fp, &spu);
    proc_info* proc = &spu;

    if (initialised != ok)
        END_PROCESS;

    err_t is_read = read_byte_code(fp, &spu);
    if (is_read != ok)
        END_PROCESS;

    proc_commands current_cmd = UNKNOWN;

    while (spu.ip < spu.prg_size)
    {
        current_cmd = (proc_commands) spu.code[spu.ip];

        if (current_cmd == HLT)
        {
            printf("main: got hlt\n");
            break;
        }

        err_t executed = execute_cmd(&spu, current_cmd);

        if (executed != ok)
            END_PROCESS;

        if (current_cmd == HLT)
            break;

        getchar();
    }

    printf("main: shutting down processor\n");
    proc_dtor(fp, &spu);
    return 0;
}
