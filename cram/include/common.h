#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ROW      128
#define COL      256
#define WORDSIZE 32
#define WORDNUM  (ROW * COL) / WORDSIZE

#ifdef __cplusplus
}
#endif

#endif
