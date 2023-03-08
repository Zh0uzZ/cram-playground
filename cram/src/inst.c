#include "inst.h"

extern VerilatedContext *contextp;
extern VCRam *top;
extern VerilatedVcdC *tfp;

bool __is_in_range(uint32_t val, int num_bits) { return (val >> num_bits) == 0; }

void __sram_write(uint32_t addr, uint32_t data) {
  if (__is_in_range(addr, 10) && __is_in_range(data, 32)) {
    top->we_i = 1;
    top->inst_i = 0;
    top->addr_i = addr;
    top->data_i = data;
  } else {
    printf("Error: input addr or data is out of range\n");
  }
}

void sram_write(uint32_t addr, uint32_t data) {
  __sram_write(addr, data);
  event_record(10);
}

uint32_t __cram_inst_gen(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  uint32_t result = tag_enable ? 0x90000000 : 0x80000000;
  result |= ((uint32_t)opcode & 0xF) << 24;
  result |= (ra & 0xFF) << 16;
  result |= (rb & 0xFF) << 8;
  result |= (rd & 0xFF);
  return result;
}

void __cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  if (__is_in_range(ra, 8) && __is_in_range(rb, 8) && __is_in_range(rd, 8)) {
    top->we_i = 0;
    top->data_i = 0;
    top->inst_i = __cram_inst_gen(opcode, ra, rb, rd, tag_enable);
  } else {
    printf("Error: one or more addrs are out of range\n");
  }
}

void cram_inst(OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, bool tag_enable) {
  __cram_inst(opcode, ra, rb, rd, tag_enable);
  event_record(10);
}

void __inst_logic (OP_TYPE opcode, uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  if ((uint32_t)opcode <= (uint32_t)XNOR) {
    for (uint32_t i = 0; i < size; i++) {
      cram_inst(opcode, (ra + i), (rb + i), (rd + i), false);
    }
  } else {
    printf("Error: not a logic instruction\n");
  }
}

void inst_and (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(AND, ra, rb, rd, size);
}

void inst_nor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(NOR, ra, rb, rd, size);
}
void inst_xor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(XOR, ra, rb, rd, size);
}
void inst_nand (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(NAND, ra, rb, rd, size);
}
void inst_or (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(OR, ra, rb, rd, size);
}
void inst_xnor (uint32_t ra, uint32_t rb, uint32_t rd, uint32_t size) {
  __inst_logic(XNOR, ra, rb, rd, size);
}
