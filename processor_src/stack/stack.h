#ifndef STACK_H
#define STACK_H

#include <assert.h>
#include <stdio.h>
#include "stack_properties.h"

st_return_err st_ctor (st_t* st, size_t capacity, md_t debug_mode);
st_return_err st_dtor (st_t* st);
st_return_err st_push (st_t* st, st_data_type value);
st_return_err st_pop (st_t* st, st_data_type* el);
st_return_err st_extend(st_t* st);

#endif
