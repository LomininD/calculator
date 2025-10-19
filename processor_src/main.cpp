#include "processor_cmd.h"
#include "processor.h"
#include "processor_init.h"

// *1000 to work with floats (add mode -f)
// TODO: calloc
// TODO: log for stack

int main(int argc, char* argv[])
{
    proc_info spu = {};
    proc_info* proc = &spu;

    err_t parsed = parse_args(argc, argv, proc);

    if (parsed != ok)
        return 0;

    printf(MAKE_BOLD("+++ PROCESSOR +++\n\n"));

    err_t initialised = proc_ctor(proc);

    if (initialised != ok)
        END_PROCESS(spu.proc_modes.debug_mode);

    err_t is_read = read_byte_code(proc);
    if (is_read != ok)
        END_PROCESS(spu.proc_modes.debug_mode);

    proc_commands current_cmd = UNKNOWN;

    while (spu.ip < spu.prg_size)
    {
        current_cmd = (proc_commands) spu.code[spu.ip];

        if (current_cmd == HLT)
        {
            printf("main: got hlt\n");
            break;
        }

        err_t executed = execute_cmd(proc, current_cmd);

        if (executed != ok)
            END_PROCESS(spu.proc_modes.debug_mode);

        if (current_cmd == HLT)
            break;

        getchar();
    }

    proc_dtor(proc);
    printf("main: shutting down processor\n");
    return 0;
}
