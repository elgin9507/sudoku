#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "shuffle.h"
#include "sudoku.h"

static int rand_range(int min, int max) {
    if (min == max) return min;
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void shuffle(void *array, size_t n, size_t size) {
    /* shuffles given array. For details:
     * https://stackoverflow.com/questions/6127503/shuffle-array-in-c#answer-6127606
     */
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;

        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

static void swap_rows(char table[][BLOCK_CELLS], int r1, int r2) {
    char tmp[BLOCK_CELLS];
    size_t size = sizeof(tmp);

    memcpy(tmp, table[r1], size);
    memcpy(table[r1], table[r2], size);
    memcpy(table[r2], tmp, size);
}

static void swap_cols(char table[][BLOCK_CELLS], int c1, int c2) {
    char tmp;
    int i;

    for (i = 0; i < NBLOCK; ++i) {
        tmp = table[i][c1];
        table[i][c1] = table[i][c2];
        table[i][c2] = tmp;
    }
}

static void swap_row_blocks(char table[][BLOCK_CELLS], int b1, int b2) {
    int i;

    for (i = 0; i < TABLE_SIZE; ++i)
        swap_rows(table, b1*TABLE_SIZE+i, b2*TABLE_SIZE+i);
}

static void swap_col_blocks(char table[][BLOCK_CELLS], int b1, int b2) {
    int i;

    for (i = 0; i < TABLE_SIZE; ++i)
        swap_cols(table, b1*TABLE_SIZE+i, b2*TABLE_SIZE+i);
}

void shuffle_seeder(char table[][BLOCK_CELLS]) {
    /* this function shuffles table seeder to
     * randomize the game board. Shuffling here
     * means to swap rows, cols in the same
     * block and row, column blocks. These
     * swappings doesn't affect game board's
     * validity.
     */
    /* ----------------------------------------- */
    /* integers that store how many times
     * rows, cols, row_blocks and col_blocks
     * are going to be swapped.
     */
    int swap_rown = rand_range(1, NBLOCK);
    int swap_coln = rand_range(1, NBLOCK);
    int swap_row_blockn = rand_range(1, NBLOCK);
    int swap_col_blockn = rand_range(1, NBLOCK);
    /* ----------------------------------------- */
    int i;
    /* swap rows */
    for (i = 0; i < swap_rown; ++i) {
        int r1 = rand_range(1, TABLE_WIDTH-1) / BLOCK_SIZE * BLOCK_SIZE;
        int r2 = r1 + rand_range(1, BLOCK_SIZE-1);
        swap_rows(table, r1, r2);
    }
    /* swap cols */
    for (i = 0; i < swap_coln; ++i) {
        int c1 = rand_range(1, TABLE_WIDTH-1) / BLOCK_SIZE * BLOCK_SIZE;
        int c2 = c1 + rand_range(1, BLOCK_SIZE-1);
        swap_cols(table, c1, c2);
    }
    /* swap row_blocks */
    for (i = 0; i < swap_row_blockn; ++i) {
        int r1 = 0;
        int r2 = r1 + rand_range(1, BLOCK_SIZE-1);
        swap_row_blocks(table, r1, r2);
    }
    /* swap col_blocks */
    for (i = 0; i < swap_col_blockn; ++i) {
        int c1 = 0;
        int c2 = c1 + rand_range(1, BLOCK_SIZE-1);
        swap_col_blocks(table, c1, c2);
    }
}
