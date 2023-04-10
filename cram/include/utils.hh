#ifndef __UTILS_HH__
#define __UTILS_HH__

#include <verilated.h>
#include <verilated_vcd_c.h>

#include "VCRam.h"

#include "common.hh"

void enable_trace_to(const char* path);
void finish();
void init();
void event_record(uint64_t event_time);

#endif
