#include "stack.h"
#include "stack_dump.h"


st_return_err st_ctor (st_t* st, size_t capacity)
{
    CTOR_ASSERT_ST(st);
    CTOR_ASSERT_CAPACITY(capacity);

    st->data = (st_data_type*) calloc (capacity + 2, sizeof(st_data_type));

    CTOR_ASSERT_DATA(st->data);

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
    PUSH_ASSERT_REALLOC(st);

    st->data[st->size + 1] = value;
    st->size++;

    CHECK_STACK(st);

    return no_error;
}


st_return_err st_pop (st_t* st, st_data_type* el)
{
    CHECK_STACK(st);

    POP_ASSERT_ELEMENTS(st);

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

    st->capacity = st->capacity * 2;
    st_data_type* new_data = (st_data_type*) realloc(st->data, (st->capacity + 2) * sizeof(st_data_type));
    st->data = new_data;

    ST_EXTEND_ASSERT_DATA(st->data);

    st->data[st->capacity + 1] = canary_value;

    CHECK_STACK(st);

    return no_error;
}
