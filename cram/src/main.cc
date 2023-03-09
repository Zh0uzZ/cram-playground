#include <verilated.h>
#include <verilated_vcd_c.h>
#include <verilated_dpi.h>

#include "VCRam.h"
#include "VCRam__Dpi.h"

#include "common.h"
#include "debug.h"
#include "inst.h"
#include "utils.h"

// DPI-C related
uint32_t* norm_mem_ptr = NULL;
uint32_t* init_mem_ptr = (uint32_t*)calloc(WORDNUM, sizeof(uint32_t));
uint32_t* carry_ptr = NULL;
uint32_t* tag_ptr = NULL;

extern "C" void set_norm_mem_ptr(const svOpenArrayHandle var) {
  norm_mem_ptr = (uint32_t*)(((VerilatedDpiOpenVar*)var)->datap());
}

extern "C" void set_norm_mem_init(svBitVecVal* var) {
  memcpy(var, (svBitVecVal*)init_mem_ptr, WORDNUM * sizeof(uint32_t));
}

extern "C" void set_carry_ptr(const svOpenArrayHandle var) {
  carry_ptr = (uint32_t*)(((VerilatedDpiOpenVar*)var)->datap());
}

extern "C" void set_tag_ptr(const svOpenArrayHandle var) {
  tag_ptr = (uint32_t*)(((VerilatedDpiOpenVar*)var)->datap());
}

VerilatedContext* contextp = new VerilatedContext;
VCRam* top = new VCRam{contextp};
VerilatedVcdC* tfp = new VerilatedVcdC;

int main(int argc, char* argv[], char* env[]) {
  if (false && argc && argv && env) {
  }

  // enable trace, save vcd file to build/main.vcd
  enable_trace_to("build/main.vcd");

  // set values for a range of memory
  set_norm_mem_range(0, 128, 0xfafafafa);
  set_norm_mem_range(128, 256, 0xafafafaf);

  // initialization
  init();

  // test cases:
  uint32_t ra = 0, rb = 32, rd = 64, step = 4;
  // logic
  // and
  inst_and(ra, rb, rd, step);
  show_norm_mem_all("logic and:");
  // nor
  inst_nor(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("logic nor:");
  // xor
  inst_xor(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("logic xor:");
  // nand
  inst_nand(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("logic nand:");
  // or
  inst_or(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("logic or:");
  // xnor
  inst_xnor(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("logic xnor:");
  // add
  inst_add(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("integer add:");
  // sub
  inst_sub(ra += step, rb += step, rd += step, step);
  show_norm_mem_all("integer sub:");

  // check if all 0xa055fa9b
  for (int i = 256; i < 384; i++) {
    assert(norm_mem_ptr[i] == 0xa055fa9b);
  }
  printf("pass\n");

  // finish
  finish();

  return 0;
}
