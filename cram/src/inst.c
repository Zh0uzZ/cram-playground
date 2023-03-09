#include "inst.h"

extern VerilatedContext *contextp;
extern VCRam *top;
extern VerilatedVcdC *tfp;

bool __is_in_range(uint32_t val, int num_bits) { return (val >> num_bits) == 0; }

void __sram_write(uint32_t addr, uint32_t data) {
  if (__is_in_range(addr, ADDR_RANGE) && __is_in_range(data, DATA_RANGE)) {
    top->we_i = SRAM_WRITE_ENABLE;
    top->inst_i = INST_DISABLE;
    top->addr_i = addr;
    top->data_i = data;
  } else {
    printf("Error: input addr or data is out of range\n");
  }
}

void sram_write(uint32_t addr, uint32_t data) {
  __sram_write(addr, data);
  event_record(CLK_CYCLE_TIME);
}

uint32_t __cram_inst_gen(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  uint32_t result = tag_enable ? INST_ENABLE | ((uint32_t)tag_enable << TAG_OFFSET) : INST_ENABLE;
  result |= ((uint32_t)opcode & OP_MASK) << OP_OFFSET;
  result |= (ra & RA_MASK) << RA_OFFSET;
  result |= (rb & RB_MASK) << RB_OFFSET;
  result |= (rd & RD_MASK) << RD_OFFSET;
  return result;
}

void __cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  if (__is_in_range(ra, RA_RANGE) && __is_in_range(rb, RB_RANGE) && __is_in_range(rd, RD_RANGE)) {
    top->we_i = SRAM_WRITE_DISABLE;
    top->data_i = EMPTY_DATA;
    top->inst_i = __cram_inst_gen(opcode, ra, rb, rd, tag_enable);
  } else {
    printf("Error: one or more addrs are out of range\n");
  }
}

void cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  __cram_inst(opcode, ra, rb, rd, tag_enable);
  event_record(CLK_CYCLE_TIME);
}

void cram_inst_empty(void) {
  top->we_i = SRAM_WRITE_DISABLE;
  top->addr_i = EMPTY_ADDR;
  top->data_i = EMPTY_DATA;
  top->inst_i = INST_DISABLE;
  event_record(CLK_CYCLE_TIME);
}

void __inst_logic(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  if ((uint32_t)opcode <= (uint32_t)XNOR) {
    for (uint32_t i = 0; i < size; i++) {
      cram_inst(opcode, (ra + i), (rb + i), (rd + i), false);
    }
  } else {
    printf("Error: not a logic instruction\n");
  }
}

void inst_and(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(AND, ra, rb, rd, size);
}

void inst_nor(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(NOR, ra, rb, rd, size);
}

void inst_xor(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(XOR, ra, rb, rd, size);
}

void inst_nand(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(NAND, ra, rb, rd, size);
}

void inst_or(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(OR, ra, rb, rd, size);
}

void inst_xnor(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(XNOR, ra, rb, rd, size);
}

void inst_add(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  cram_inst(CC, UNUSED, UNUSED, UNUSED, false);
  for (int32_t i = (size - 1); i >= 0; i--) {
    cram_inst(ADD, (ra + i), (rb + i), (rd + i), false);
  }
}

void inst_sub(uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  cram_inst(SC, UNUSED, UNUSED, UNUSED, false);
  for (int32_t i = 0; i < size; i++) {
    cram_inst(INV, (rb + i), UNUSED, (rb + i), false);
  }
  for (int32_t i = (size - 1); i >= 0; i--) {
    cram_inst(ADD, (ra + i), (rb + i), (rd + i), false);
  }
}
