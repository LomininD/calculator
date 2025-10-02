#include "stack_dump.h"


// verifier looks for errors
verifier_output st_verify(st_t* st)
{
    int found_errors = 0;

    if (st == NULL)
    {
        st_dump(st);
        return not_verified;
    }

    if (st->size > st->capacity)
    {
        st->error = stack_overflow;
        found_errors += 1;
    }

    if (st->data == NULL)
    {
        st->error = no_data;
        st_dump(st);
        return not_verified;
    }

    // pay attention to if order: some checks may be impossible

    if (st->data[0] != canary_value || st->data[st->capacity + 1] != canary_value)
    {
        st->error = canary_fault;
        found_errors += 1;
    }

    if (found_errors == 0)
    {
        return verified ;
    }
    else
    {
        st_dump(st);
        //printf("error num %d\n", found_errors);
        return not_verified;
    }
}


// st_dump checks if any errors were registered and sends log
void st_dump(st_t* st)
{
    printf("-----------------------------------------------------------\n");

    if (st == NULL)
        printf(MAKE_BOLD_RED("ERROR: ") "[from verifier] -> " \
                                    " stack not found (got NULL pointer)\n");
    else
    {
        if (st->error == no_data)
            printf(MAKE_BOLD_RED("ERROR: ") "[from verifier] -> "  \
                                "stack data not found (got NULL pointer)\n\n");

        if (st->error == canary_fault)
            printf(MAKE_BOLD_RED("UNAUTHORIZED ACCESS TO DATA: ") \
                            "[from verifier] -> canary protection triggered\n\n");
            // if canary protection triggered should we print data in dump?

        if (st->error == stack_overflow)
            printf(MAKE_BOLD_RED("ERROR: ") "[from verifier] -> stack overflow\n\n");

        print_st_info(st);
    }

    printf("-----------------------------------------------------------\n");
}

// check(error) - gets binary error var and checks if requested
// error has occurred using switch
void print_st_info(st_t* st)
{

    if (st != NULL)
    {
        printf(MAKE_BOLD("stack") "  [%p]\n", st);
        printf("{\n");
        printf("\tsize = %zu\n", st->size);
        printf("\tcapacity = %zu\n\n", st->capacity);
    }
    else
    {
        printf(MAKE_BOLD("stack") "  [not found]\n");
    }

    print_st_data(st);

    printf("}\n");
}


void print_st_data(st_t* st)
{
    assert(st != NULL);

    if (st->data != NULL)
        {
            printf("\tdata  [%p]\n", st->data);
            printf("\t{\n");

            printf(MAKE_GREY("\t\t [0] = %x (canary protection)\n"), st->data[0]);

            if (st->error != stack_overflow)
            {
                print_st_values(st);
            }
            else
            {
                printf("\t\t [overflowed]\n");
            }

            printf(MAKE_GREY("\t\t [%zu] = %x (canary protection)\n"),
                                st->capacity + 1, st->data[st->capacity+1]);

            printf("\t}\n");
        }
        else
        {
            printf("\tdata  [not found]\n");
        }
}

void print_st_values(st_t* st)
{
    assert(st != NULL);

    for (size_t i = 0; i < min(st->capacity, st_output_size / 2); i++)
    {
        if (i < st->size)
            printf("\t\t*[%zu] = %d\n", i + 1, st->data[i+1]);
        else
            printf("\t\t [%zu] = [empty]\n", i + 1);
    }

    if (st_output_size < st->capacity)
    {
        printf("\t\t ...\n");
    }

    for (size_t i = max(st_output_size / 2, st->capacity - st_output_size / 2); \
                                                        i < st->capacity; i++)
    {
        if (i < st->size)
            printf("\t\t*[%zu] = %d\n", i + 1, st->data[i+1]);
        else
            printf("\t\t [%zu] = [empty]\n", i + 1);
    }
}
