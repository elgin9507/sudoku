#include <stdbool.h>

#ifndef __SUDOKU_H
#define __SUDOKU_H

/* constants related to table size */
#define TABLE_SIZE 3
#define NBLOCK (TABLE_SIZE) * (TABLE_SIZE)
#define BLOCK_SIZE 3
#define BLOCK_CELLS (BLOCK_SIZE) * (BLOCK_SIZE)
#define TABLE_WIDTH (TABLE_SIZE) * (BLOCK_SIZE)
/* --------------------------------- */

/* sudoku table structures */
typedef struct cell {
    int digit;
    bool filled;
} cell;

typedef struct block {
    cell *cells;
} block;

typedef struct board {
    block *blocks;
} board;
/* --------------------------------- */

/* API */
void init_board(board*);
void load_from_seed(board*);
void print_board(board);
void print_table_seeder(void); // TODO temporary delete afterward
bool is_valid_board(board);
void empty_cells(board*);
void destroy_board(board*);
bool board_is_full(board);
void prep_board();
void start_game();
void usage();
/* --------------------------------- */

/* user input handlers */
void handle_cell_write(char*, char*);
void handle_cell_clear(char*);
void handle_game_restart();
/* --------------------------------- */

#endif /* __SUDOKU_H */
