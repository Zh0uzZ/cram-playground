#ifndef __COMMON_HH__
#define __COMMON_HH__

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
}
#endif

#define ROW      128
#define COL      256
#define WORDSIZE 32
#define WORDNUM  (ROW * COL) / WORDSIZE

#define CLK_FLIP_TIME   5
#define CLK_CYCLE_TIME  (2 * CLK_FLIP_TIME)
#define CLK_LEVEL_LOW   0
#define CLK_LEVEL_HIGH  1
#define RESET_TIME      20
#define RESET_ENABLE    0
#define RESET_DISABLE   1
#define WAIT_TIME       (RESET_TIME + CLK_FLIP_TIME + 2)

#define SRAM_WRITE_ENABLE  1
#define SRAM_WRITE_DISABLE 0
#define EMPTY_ADDR         0
#define EMPTY_DATA         0
#define INST_ENABLE        0x80000000
#define INST_DISABLE       0

#endif
