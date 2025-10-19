#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processor.h"

// make printf macros for debug
// stack information and program info in debug file if mode is not debug
// if function couldn't complete the direct task it prints error message and returns error
// if function gets error message from other function it redirects it to the
// function from which was called
//TODO: split output streams ???
// refactor stack only in case of splitting streams


err_t open_file(FILE** fp, int argc, char* argv[])
{
    assert(argv != NULL);

    if (argc == 2)
    {
        *fp = fopen(argv[1], "r");
        if (*fp == NULL)
        {
            printf("open_file: " MAKE_BOLD_RED("ERROR:") " could not open the file\n");
            printf(MAKE_GREY("Note: file name may be incorrect\n"));
            return error;
        }
    }
    else
    {
        printf("open_file: " MAKE_BOLD_RED("ERROR:") " no file was provided\n");
        return error;
    }

    return ok;
}


err_t read_byte_code(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

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


err_t execute_cmd(proc_info* proc, proc_commands cmd) // refactor with struct
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


err_t proc_ctor(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    printf("proc_ctor: began initialising processor\n");

    size_t capacity = 10;
    err_t initialised = initialise_stack(capacity, &(proc->st));
    if (initialised != ok)
        return error;

    proc->ip = 0;
    proc->prg_size = 0;

    err_t prepared = prepare_file(fp, proc);
    if (prepared != ok)
        return error;

    printf("proc_ctor: processor initialised\n\n");
    return ok;
}


err_t prepare_file(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    int c = 0;
    int correct_symbols = 0;

    err_t signature_ok = check_file_signature(fp, proc);

    if (signature_ok != ok)
        return error;

    err_t version_ok = check_file_version(fp, proc);

    if (version_ok != ok)
        return error;

    int prg_size = 0;
    fscanf(fp, "%d", &prg_size);

    return ok;
}


err_t check_file_signature(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    int c = 0;
    int correct_symbols = 0;

    fscanf(fp, "%d", &c);
    if (c == 'L')
        correct_symbols++;
    fscanf(fp, "%d", &c);
    if (c == 'M')
        correct_symbols++;
    fscanf(fp, "%d", &c);
    if (c == 'D')
        correct_symbols++;

    if (correct_symbols == 3)
    {
        printf("check_file_signature: file signature verified\n");
        return ok;
    }
    else
    {
        printf("check_file_signature: " MAKE_BOLD_RED("ERROR:") " wrong file signature\n");
        return error;
    }
}


err_t check_file_version(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    int file_version = 0;
    fscanf(fp, "%d", &file_version);
    if (file_version < version)
    {
        printf("check_file_version: " MAKE_BOLD_RED("ERROR:") " assembly version is old\n");
        return error;
    }
    else if (file_version > version)
    {
        printf("check_file_version: " MAKE_BOLD_RED("ERROR:") " processor version is old\n");
        return error;
    }
    else
    {
        printf("check_file_version: assembly version verified\n");
        return ok;
    }
}


err_t initialise_stack(size_t capacity, st_t* st)
{
    assert(st != NULL);

    st_return_err created = st_ctor(st, capacity);

    if (created == no_error)
    {
        printf("initialise_stack: stack created\n");
        return ok;
    }
    else
    {
        printf("initialise_stack: problem occurred while creating stack\n");
        return error;
    }
}


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


err_t proc_dtor(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    printf("proc_dtor: started termination\n");

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
    printf("proc_dtor: done\n");
    return ok;
}

