#ifndef USEFUL_DEFINES_H
#define USEFUL_DEFINES_H

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

#define $$ fprintf(stderr, "line: %d\n", __LINE__);

#define $$$(...) { fprintf(stderr, "%s... ", #__VA_ARGS__); __VA_ARGS__; fprintf(stderr, "done\n"); }

#endif
