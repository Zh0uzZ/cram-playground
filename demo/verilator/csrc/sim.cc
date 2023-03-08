#include <stdbool.h>
#include <verilated.h>

// from the module name: hw -> Vhw.h
#include "Vhw.h"

VerilatedContext *contextp = new VerilatedContext;

// from the module name: hw -> Vhw
Vhw *top = new Vhw{contextp};

// Circuit emulation helpers, from the simple demo
#define END_CYCLE                                                              \
  ({                                                                           \
    end_cycle();                                                               \
    putchar('\n');                                                             \
    fflush(stdout);                                                            \
    sleep(1);                                                                  \
  })

// fix PIRNT(X) in the simple demo
#define PRINT(X) printf(#X " = %d; ", top->X)

void end_cycle() {
  PRINT(A);
  PRINT(B);
  PRINT(C);
  PRINT(D);
  PRINT(E);
  PRINT(F);
  PRINT(G);
}

int main(int argc, char **argv) {
  if (false && argc && argv) {
  }

  while (!contextp->gotFinish()) {
    top->clk = (top->clk == 1 ? 0 : 1);
    top->eval();
    END_CYCLE;
  }

  delete top;
  delete contextp;
  return 0;
}
