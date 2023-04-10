#include "debug.hh"

extern uint32_t* norm_mem_ptr;
extern uint32_t* init_mem_ptr;
extern uint32_t* carry_ptr;
extern uint32_t* tag_ptr;

void show_norm_mem_addr(int addr, bool print_line) {
  printf("[%04d: 0x%08x]", addr, norm_mem_ptr[addr]);
  printf(print_line ? "\n" : "");
}

void show_norm_mem_range(int from_addr, int to_addr, bool show_cary, bool show_tag) {
  for (int i = ROW; i > 0; i--) {
    for (int j = 0;  j < (COL / WORDSIZE); j++) {
      int addr = (i - 1) + j * ROW;
      if (addr >= from_addr && addr < to_addr) {
        show_norm_mem_addr(addr, false);
      } else {
        printf("%*s", 18, " ");
      }
      printf((j == ((COL / WORDSIZE) - 1)) ? "" : " ");
    }
    printf(show_carry ? " %01d" : "", carry_ptr[i - 1]);
    printf(show_tag ? " %01d" : "", tag_ptr[i - 1]);
    printf("\n");
  }
}

void show_norm_mem_all(const char* info) {
  printf("# content of CRam: (followed by carry flag and tag flag)\n");
  printf("%s\n", info);
  show_norm_mem_range(0, WORDNUM, true, true);
}

void set_norm_mem_addr(int addr, uint32_t value) {
  init_mem_ptr[addr] = value;
}

void set_norm_mem_range(int from_addr, int to_addr, uint32_t value) {
  for (int i = from_addr; i < to_addr; i++) {
    set_norm_mem_addr(i, value);
  }
}

void set_norm_mem_all(uint32_t value) {
  set_norm_mem_range(0, WORDNUM, value);
}

void show_carry(void) {
  for (int i = ROW; i > 0; i--) {
    printf("[carry: %01d]\n", carry_ptr[i - 1]);
  }
}

void show_tag(void) {
  for (int i = ROW; i > 0; i--) {
    printf("[tag:   %01d]\n", tag_ptr[i - 1]);
  }
}
