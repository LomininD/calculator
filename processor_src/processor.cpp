#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processor.h"
#include "processor_init.h"

// make printf macros for debug
// TODO: split output streams ???

err_t proc_ctor(proc_info* proc)
{
    assert(proc != NULL);

    printf("proc_ctor: began initialising processor\n");

    size_t capacity = 10;
    err_t initialised = initialise_stack(capacity, &(proc->st));
    if (initialised != ok)
        return error;

    proc->ip = 0;
    proc->prg_size = 0;

    err_t prepared = prepare_file(proc->byte_code_file);
    if (prepared != ok)
        return error;

    printf("proc_ctor: processor initialised\n\n");
    return ok;
}


err_t read_byte_code(proc_info* proc)
{
    assert(proc != NULL);

    FILE* fp = proc->byte_code_file;

    assert(fp != NULL);

    printf("started reading\n");

    int cmd = 0;
    int i = 0;
    int scanned = 0;

    while (true)
    {
        scanned = fscanf(fp, "%d", &cmd);

        if (scanned == -1)
            break;

        if (i == max_byte_code_len)
        {
            printf("read_code: max byte code len exceeded\n");
            return error;
        }

        //printf("scanned_cmd: %d\n", cmd);
        proc->prg_size++;
        proc->code[i] = cmd;
        i++;
    }

    //printf("prg_size: %zu\n", prg_size);
    return ok;
}


err_t execute_cmd(proc_info* proc, proc_commands cmd)
{
    assert(proc != NULL);

    err_t executed = ok;

    switch (cmd)
    {
        case PUSH:
            executed = proc_push(proc);
            break;
        case PUSHREG:
            executed = proc_pushreg(proc);
            break;
        case POPREG:
            executed = proc_popreg(proc);
            break;
        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case SQRT:
            executed = proc_calc(proc, cmd);
            break;
        case IN:
            executed = proc_in(proc);
            break;
        case OUT:
            executed = proc_out(proc);
            break;

        case JMP:
            executed = proc_jmp(proc);
            break;

        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
            executed = proc_cond_jmp(proc, cmd);
            break;

        default:
            printf("execute_cmd: unknown command (cmd = %d, ip = %zu), cannot execute\n", cmd, proc->ip);
    }

    return executed;
}


/*
err_t check_ip(proc_info* proc)
{
    assert(proc != NULL);

    if (proc->ip >= proc->prg_size)
    {
        printf("check_ip: " MAKE_BOLD_RED("ERROR:") " ip value (%zu) is bigger than byte code size (%zu)\n", proc->ip, proc->prg_size);
        return error;
    }
    else
    {
        printf("check_ip: ip value (%zu) is ok\n", proc->ip);
        return ok;
    }
}
*/


err_t proc_dtor(proc_info* proc)
{
    assert(proc != NULL);

    FILE* fp = proc->byte_code_file;

    assert(fp != NULL);

    printf("proc_dtor: began termination\n");

    st_return_err terminated = st_dtor(&proc->st);

    if (terminated == no_error)
    {
        printf("proc_dtor: stack destroyed\n");
    }
    else
    {
        printf("execute_cmd: " MAKE_BOLD_RED("ERROR:") " hlt failed\n");
        return error;
    }

    fclose(fp);
    printf("proc_dtor: done termination\n");
    return ok;
}

