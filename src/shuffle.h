#include <stdlib.h>

#include "sudoku.h"

#ifndef __SHUFFLE_H
#define __SHUFFLE_H

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define print_type(count, stmt) \
    do { \
        printf("["); \
        for (size_t i = 0; i < (count); ++i) { \
            stmt; \
        } \
        printf("]"); \
    } while(0)

static int rand_range(int, int);
void shuffle(void*, size_t, size_t);
static void swap_rows(char[][BLOCK_CELLS], int, int);
static void swap_cols(char[][BLOCK_CELLS], int, int);
static void swap_row_blocks(char[][BLOCK_CELLS], int, int);
static void swap_col_blocks(char[][BLOCK_CELLS], int, int);
void shuffle_seeder(char[][BLOCK_CELLS]);

#endif /* __SHUFFLE_H */
