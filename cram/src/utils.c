#include "utils.h"

extern VerilatedContext *contextp;
extern VCRam *top;
extern VerilatedVcdC *tfp;
extern uint32_t* init_mem_ptr;

void enable_trace_to(const char* path) {
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open(path);
}

void finish() {
  tfp->close();
  top->final();
  delete top;
  delete contextp;
  delete tfp;
  free(init_mem_ptr);
}

void __event_record(uint64_t reset_time, uint64_t wait_time) {
  while (contextp->time() < wait_time && !contextp->gotFinish()) {
    contextp->timeInc(1);
    if (contextp->time() > reset_time) {
      top->rst_ni = RESET_DISABLE;
    }
    if (contextp->time() % CLK_FLIP_TIME == 0) {
      top->clk_i ^= CLK_LEVEL_HIGH;
    }
    top->eval();
    tfp->dump(contextp->time());
  }
}

void init() {
  // init input
  top->we_i = SRAM_WRITE_DISABLE;
  top->inst_i = INST_DISABLE;
  top->addr_i = EMPTY_ADDR;
  top->data_i = EMPTY_DATA;
  // init clock and reset
  top->rst_ni = RESET_ENABLE;
  top->clk_i = CLK_LEVEL_LOW;
  top->eval();
  top->clk_i = CLK_LEVEL_HIGH;
  top->eval();
  tfp->dump(contextp->time());
  // reset and wait
  __event_record(RESET_TIME, WAIT_TIME);
}

void event_record(uint64_t event_time) {
  uint64_t wait_time = contextp->time() + event_time;
  __event_record(RESET_TIME, wait_time);
}
