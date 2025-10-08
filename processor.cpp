#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "processor_properties.h"
#include "processor_cmd.h"
#include "stack/stack.h"
#include "stack/font_styles.h"
#include "stack/stack_dump.h"

struct proc_info
{
    int code[100]; // refactor using calloc and realloc or store file size in file
    size_t prg_size;
    st_t st;
    int registers[8];
    size_t ip;
};

// make printf macros for debug
// stack information and program info in debug file if mode is not debug

err_t open_file(FILE** fp, int argc, char* argv[]);
err_t proc_ctor(FILE* fp, proc_info* proc);
err_t prepare_file(FILE* fp, proc_info* proc);
err_t execute_cmd(proc_info* proc, proc_commands cmd);
err_t initialise_stack(size_t capacity, st_t* st);
err_t check_ip(proc_info* proc);
int is_jmp_type(proc_commands cmd);
void read_byte_code(FILE* fp, proc_info* proc);

// if function couldn't complete the direct task it prints error message and returns error
// if function gets error message from other function it redirects it to the
// function from which was called
//TODO: split output streams ???
// refactor stack only in case of splitting streams

int main(int argc, char* argv[])
{
    printf(MAKE_BOLD("+++ PROCESSOR +++\n\n"));

    FILE* fp = NULL;

    err_t opened = open_file(&fp, argc, argv);

    if (opened != ok)
    {
        printf("main: terminating process due to error\n"); // macros
    return 0; // macros
    }

    proc_info proc = {};
    err_t initialised = proc_ctor(fp, &proc);

    if (initialised != ok)
    {
        fclose(fp); // dtor
        printf("main: terminating process due to error\n"); // macros
        return 0;
    }

    read_byte_code(fp, &proc);

    proc_commands current_cmd = UNKNOWN;

    while (proc.ip < proc.prg_size)
    {
        current_cmd = (proc_commands) proc.code[proc.ip];
        err_t executed = execute_cmd(&proc, current_cmd);

        if (executed != ok)
        {
            fclose(fp); // dtor
            printf("main: terminating process due to error\n"); //macros
            return 0;
        }

        if (current_cmd == HLT)
        {
            fclose(fp); // dtor
            printf("main: shutting down processor\n");
            return 0;
        }

        if (!is_jmp_type(current_cmd))
            proc.ip++;

        getchar(); // optionally
    }

    printf("main: forced process termination, no HLT got\n");
    fclose(fp); // dtor
    return 0;
}



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


void read_byte_code(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    int cmd = 0;
    int i = 0;
    int scanned = 0;

    while (true) // add read checking (corrupted byte code) and add new fuction
    {
        scanned = fscanf(fp, "%d", &cmd);

        if (scanned == -1)
            break;

        //printf("scanned_cmd: %d\n", cmd);
        proc->prg_size++;
        proc->code[i] = cmd;
        i++;
    }
    //printf("prg_size: %zu\n", prg_size);
}


err_t execute_cmd(proc_info* proc, proc_commands cmd) // refactor with struct
{
    assert(proc != NULL);

    err_t executed = ok;

    switch (cmd)
    {
        case PUSH:
            executed = proc_push(&proc->st, proc->code, &proc->ip);
            break;

        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case SQRT:
            executed = proc_calc(&proc->st, cmd);
            break;

        case OUT:
            executed = proc_out(&proc->st);
            break;

        case JMP:
            executed = proc_jmp(&proc->st, proc->code, proc->prg_size, &proc->ip);
            break;

        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
            executed = proc_cond_jmp(&proc->st, proc->code, proc->prg_size, &proc->ip, cmd);
            break;

        case HLT:
            executed = proc_hlt(&proc->st);
            break;

        default:
            printf("execute_cmd: unknown command (cmd = %d, ip = %zu), cannot execute\n", cmd, proc->ip);
    }

    if (executed == ok)
    {
        return ok;
    }
    else
    {
        return error;
    }
}


err_t proc_ctor(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    printf("proc_ctor: began initialising processor\n");

    // initialise stack, structure, check version, signature and get prog_size
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
        printf("prepare_file: file signature verified\n");
    }
    else
    {
        printf("prepare_file: " MAKE_BOLD_RED("ERROR:") " wrong file signature\n");
        return error;
    }

    int file_version = 0;
    fscanf(fp, "%d", &file_version);
    if (file_version == version)
    {
        printf("prepare_file: assembly version verified\n");
    }
    else
    {
        printf("prepare_file: " MAKE_BOLD_RED("ERROR:") " assembly version is old\n");
        return error;
    }

    return ok;
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
        return stack_error;
    }
}


int is_jmp_type(proc_commands cmd)
{
    switch (cmd)
    {
        case JMP:
        case JB:
        case JBE:
        case JA:
        case JAE:
        case JE:
        case JNE:
            return 1;
            break;
        default:
            return 0;
            break;
    };
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

