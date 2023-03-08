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
      top->rst_ni = 1;
    }
    if (contextp->time() % 5 == 0) {
      top->clk_i = (top->clk_i == 1 ? 0 : 1);
    }
    top->eval();
    tfp->dump(contextp->time());
  }
}

void init(uint64_t wait_time) {
  // init input
  top->we_i = 0;
  top->inst_i = 0;
  top->addr_i = 0;
  top->data_i = 0;
  // init clock and reset
  top->rst_ni = 0;
  top->clk_i = 0;
  top->eval();
  top->clk_i = 1;
  top->eval();
  tfp->dump(contextp->time());
  // reset after 20, record at time 25
  __event_record(20, 25);
}

void event_record(uint64_t event_time) {
  uint64_t wait_time = contextp->time() + event_time;
  __event_record(20, wait_time);
}
