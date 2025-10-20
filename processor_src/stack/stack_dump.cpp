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
    md_t debug_mode = st->debug_mode; // !

    printf_log_msg(debug_mode, "-----------------------------------------------------------\n");

    if (st == NULL)
        printf_err(debug_mode, "[from verifier] -> " \
                                    " stack not found (got NULL pointer)\n");
    else
    {
        if (st->error == no_data)
            printf_err(debug_mode, "[from verifier] -> "  \
                                "stack data not found (got NULL pointer)\n\n");

        if (st->error == canary_fault)
            printf_err(debug_mode, "UNAUTHORIZED ACCESS TO DATA: " \
                            "[from verifier] -> canary protection triggered\n\n");

        if (st->error == stack_overflow)
            printf_err(debug_mode,  "[from verifier] -> stack overflow\n\n");

        print_st_info(st);
    }

    printf_log_msg(debug_mode, "-----------------------------------------------------------\n");
}

void print_st_info(st_t* st)
{
    md_t debug_mode = st->debug_mode;

    if (st != NULL)
    {
        printf_log_msg(debug_mode, "stack  [%p]\n", st);
        printf_log_msg(debug_mode, "{\n");
        printf_log_msg(debug_mode, "\tsize = %zu\n", st->size);
        printf_log_msg(debug_mode, "\tcapacity = %zu\n\n", st->capacity);
    }
    else
    {
        printf_log_msg(debug_mode, "stack  [not found]\n");
    }

    print_st_data(st);

    printf_log_msg(debug_mode, "}\n");
}


void print_st_data(st_t* st)
{
    assert(st != NULL);

    md_t debug_mode = st->debug_mode;

    if (st->data != NULL)
        {
            printf_log_msg(debug_mode, "\tdata  [%p]\n", st->data);
            printf_log_msg(debug_mode, "\t{\n");

            printf_log_grey(debug_mode, "\t\t [0] = %x (canary protection)\n", st->data[0]);

            if (st->error != stack_overflow)
            {
                print_st_values(st);
            }
            else
            {
                printf_log_msg(debug_mode, "\t\t [overflowed]\n");
            }

            printf_log_grey(debug_mode, "\t\t [%zu] = %x (canary protection)\n",
                                st->capacity + 1, st->data[st->capacity+1]);

            printf_log_msg(debug_mode, "\t}\n");
        }
        else
        {
            printf_log_msg(debug_mode, "\tdata  [not found]\n");
        }
}

void print_st_values(st_t* st)
{
    assert(st != NULL);

    md_t debug_mode = st->debug_mode;

    for (size_t i = 0; i < min(st->capacity, st_output_size / 2); i++)
    {
        if (i < st->size)
            printf_log_msg(debug_mode, "\t\t*[%zu] = %d\n", i + 1, st->data[i+1]);
        else
            printf_log_msg(debug_mode, "\t\t [%zu] = [empty]\n", i + 1);
    }

    if (st_output_size < st->capacity)
    {
        printf_log_msg(debug_mode, "\t\t ...\n");
    }

    for (size_t i = max(st_output_size / 2, st->capacity - st_output_size / 2); \
                                                        i < st->capacity; i++)
    {
        if (i < st->size)
            printf_log_msg(debug_mode, "\t\t*[%zu] = %d\n", i + 1, st->data[i+1]);
        else
            printf_log_msg(debug_mode, "\t\t [%zu] = [empty]\n", i + 1);
    }
}
