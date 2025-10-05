#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "processor_properties.h"
#include "stack/stack.h"


void calc_push(st_t* st, int* code, size_t* ip);
void calc_add(st_t* st);
void calc_sub(st_t* st);
void calc_mult(st_t* st);
void calc_div(st_t* st);
void calc_sqrt(st_t* st);
void calc_out(st_t* st);
void calc_hlt(st_t* st);

#endif
