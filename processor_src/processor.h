#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h"
#include "stack/font_styles.h"
#include "stack/stack_dump.h"
#include "processor_properties.h"
#include "processor_cmd.h"

err_t open_file(FILE** fp, int argc, char* argv[]);
err_t proc_ctor(FILE* fp, proc_info* proc);
err_t proc_dtor(FILE* fp, proc_info* proc);
err_t prepare_file(FILE* fp, proc_info* proc);
err_t check_file_signature(FILE* fp, proc_info* proc);
err_t check_file_version(FILE* fp, proc_info* proc);
err_t execute_cmd(proc_info* proc, proc_commands cmd);
err_t initialise_stack(size_t capacity, st_t* st);
err_t check_ip(proc_info* proc);
err_t read_byte_code(FILE* fp, proc_info* proc);

#define $ {                                                                 \
    verifier_output res = st_verify(&proc->st);                             \
    if (res != verified)                                                    \
    {                                                                       \
        printf("failed to verify\n");                                       \
        st_dump(&proc->st);                                                 \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        printf("verified %s:%d\n", __FILE__, __LINE__);                     \
    }                                                                       \
}

#define END_PROCESS                                     \
do{                                                     \
    proc_dtor(fp, &spu);                               \
    printf("main: terminating process due to error\n"); \
    return 0;                                           \
}                                                       \
while(0)

#endif
