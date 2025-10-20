#ifndef PROCESSOR_DUMP_H
#define PROCESSOR_DUMP_H

#include "../shared/processor_properties.h"
#include <stdio.h>

const int code_width = 10;
const size_t max_number_size = 6; // change if numbers are too big

void spu_dump(proc_info* proc);
void print_code(proc_info* proc);
void print_registers(proc_info* proc);
void put_number(int number, md_t debug_mode, bool is_current_cmd);


#endif
