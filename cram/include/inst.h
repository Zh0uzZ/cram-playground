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

#define ADDR_RANGE 10
#define DATA_RANGE 32

#define TAG_OFFSET 28
#define OP_OFFSET  24
#define RA_OFFSET  16
#define RB_OFFSET  8
#define RD_OFFSET  0

#define OP_RANGE 4
#define RA_RANGE 8
#define RB_RANGE 8
#define RD_RANGE 8

#define RANGE_TO_MASK(RANGE) ((1 << RANGE) - 1)
#define OP_MASK RANGE_TO_MASK(OP_RANGE)
#define RA_MASK RANGE_TO_MASK(RA_RANGE)
#define RB_MASK RANGE_TO_MASK(RB_RANGE)
#define RD_MASK RANGE_TO_MASK(RD_RANGE)

#define UNUSED 0

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
  LDT, STC, STT, SC, CC, C2T = OP_MASK
} OP_TYPE;

void sram_write(uint32_t addr, uint32_t data);

// single-cycle instruction
void cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable);
void cram_inst_empty(void);

// multi-cycle instrutions
// logic:
void inst_and (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_nor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_xor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_nand(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_or  (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_xnor(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
// integer:
void inst_add(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
void inst_sub(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size);
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
