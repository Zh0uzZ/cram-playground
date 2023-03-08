#ifndef __INST_H
#define __INST_H

#include <verilated.h>
#include <verilated_vcd_c.h>

#include "VCRam.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "utils.h"

typedef enum {
  // logic
  AND, OR, XOR, NAND, NOR, XNOR,
  // arithmetic
  ADD,
  // shift
  CPY, INV,
  // comparison
  EQU,
  // utility
  LDT, STC, STT, SC, CC, C2T = 0xf
} OP_TYPE;

void sram_write(uint32_t addr, uint32_t data);

// single-cycle instruction
void cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable);

// multi-cycle instrutions
// logic:
void inst_and (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_nor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_xor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_nand(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_or  (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_xnor(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// integer:
// void inst_add(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// void inst_sub(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// void inst_mul(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// void inst_div(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// 32-bit float point:
// TODO

// comparison:
// TODO

#ifdef __cplusplus
}
#endif

#endif
