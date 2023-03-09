#ifndef __UTILS_H
#define __UTILS_H

#include <verilated.h>
#include <verilated_vcd_c.h>

#include "VCRam.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void enable_trace_to(const char* path);
void finish();
void init();
void event_record(uint64_t event_time);

#ifdef __cplusplus
}
#endif

#endif
