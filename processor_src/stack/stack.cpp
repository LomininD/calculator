#include "stack.h"
#include "stack_dump.h"


st_return_err st_ctor (st_t* st, size_t capacity, md_t debug_mode)
{
    if (st == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from st_ctor] -> no access to stack "
                                                    "(got NULL pointer)\n");
        return no_stack;
    }

    st->debug_mode = debug_mode;

    st->data = (st_data_type*) calloc (capacity + 2, sizeof(st_data_type));

    if (st->data == NULL)
    {
        printf_err(debug_mode, "[from st_ctor] -> could not allocate "
                                            "memory for stack data\n");
        return no_memory;
    }

    st->size = 0;
    st->capacity = capacity;
    st->error = st_ok;

    //setting up canary protection
    st->data[0] = canary_value;
    st->data[capacity + 1] = canary_value;

    CHECK_STACK(st);

    return no_error;
}


st_return_err st_push (st_t* st, st_data_type value)
{
    CHECK_STACK(st);

    if (st->size == st->capacity)
    {
        st_return_err err = st_extend(st);
        if (err != no_error)
            return err;
    }

    st->data[st->size + 1] = value;
    st->size++;

    CHECK_STACK(st);

    return no_error;
}


st_return_err st_pop (st_t* st, st_data_type* el)
{
    CHECK_STACK(st);

    if (st->size == 0)
    {
        printf_err(st->debug_mode, "[from st_pop] -> pop failed, "
                                                "no elements in stack\n");
        st_dump(st);
        return no_elements;
    }

    *el = st->data[--(st->size) + 1];

    CHECK_STACK(st);

    return no_error;
}


st_return_err st_dtor (st_t* st)
{
    CHECK_STACK(st);

    free(st->data);

    return no_error;
}


st_return_err st_extend(st_t* st)
{
    CHECK_STACK(st);

    if (st->capacity < 10)
        st->capacity = 10;
    else
        st->capacity = st->capacity * 2;

    st_data_type* new_data = (st_data_type*) realloc(st->data, (st->capacity + 2) * sizeof(st_data_type));

    if (new_data == NULL)
    {
        printf_err(st->debug_mode, "[from st_extend from st_push] -> "
            "could not reallocate memory for stack data (got NULL pointer)\n");
            return no_memory;
    }

    st->data = new_data;

    st->data[st->capacity + 1] = canary_value;

    CHECK_STACK(st);

    return no_error;
}
