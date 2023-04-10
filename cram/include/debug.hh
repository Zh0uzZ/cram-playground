#ifndef __DEBUG_HH__
#define __DEBUG_HH__

#include "common.hh"

void show_norm_mem_addr(int addr, bool print_line);
void show_norm_mem_range(int from_addr, int to_addr, bool show_carry, bool show_tag);
void show_norm_mem_all(const char* info);

void set_norm_mem_addr(int addr, uint32_t value);
void set_norm_mem_range(int from_addr, int to_addr, uint32_t value);
void set_norm_mem_all(uint32_t value);

void show_carry(void);
void show_tag(void);

#endif
