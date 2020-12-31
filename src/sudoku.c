#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "sudoku.h"
#include "shuffle.h"
#include "utils.h"
#include "fort.h"
#include "render.h"
#include "parser.h"

board game_board;
/* table seeder will be used to generate
 * game board. The board is represented
 * 2D array of tokens. Characters 'a-i'
 * are used as tokens for this purpose.
 * The tokens will be replaced by actual
 * digits (see below token map).
 */
char table_seeder[NBLOCK][BLOCK_CELLS] = {
    "abcdefghi",
    "defghiabc",
    "ghiabcdef",
    "bcaefdhig",
    "efdhigbca",
    "higbcaefd",
    "cabfdeigh",
    "fdeighcab",
    "ighcabfde",
};
char token_map[] = "123456789";

void init_board(board *_board) {
    /* allocating memory for blocks and
     * their cells here. All cells are
     * initialized with 'digit' 0 and
     * 'filled' true.
     */
    block *blocks = NULL;
    int i, j;

    blocks = (block*)malloc(sizeof(block) * NBLOCK);
    for (i = 0; i < NBLOCK; ++i) {
        blocks[i].cells = (cell*)malloc(sizeof(cell) * BLOCK_CELLS);
        for (j = 0; j < BLOCK_CELLS; ++j) {
            blocks[i].cells[j].digit = 0;
            blocks[i].cells[j].filled = true;
        }
    }
    _board->blocks = blocks;
}

void load_from_seed(board *_board) {
    /* converting tokens to digits here
     * to populate game board.
     */
    int i, j, digit;

    for (i = 0; i < NBLOCK; ++i)
        for (j = 0; j < BLOCK_CELLS; ++j) {
            digit = token_map[table_seeder[i][j]-'a'] - '0';
            _board->blocks[i].cells[j].digit = digit;
        }
}

bool board_is_full(board _board) {
    int i, j;

    for (i = 0; i < NBLOCK; ++i)
        for (j = 0; j < BLOCK_CELLS; ++j)
            if (!_board.blocks[i].cells[j].filled)
                return false;

    return true;
}

void print_board(board _board) {
    int i, j;
    ft_table_t *table = ft_create_table();
    cell _cell;
    char cell_str[2];

    #define TMARGIN_X 2
    #define TMARGIN_Y 2
    ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, TMARGIN_X);
    ft_set_tbl_prop(table, FT_TPROP_RIGHT_MARGIN, TMARGIN_X);
    ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, TMARGIN_Y);
    ft_set_tbl_prop(table, FT_TPROP_BOTTOM_MARGIN, TMARGIN_Y);
    ft_set_border_style(table, FT_SOLID_ROUND_STYLE);

    cell_str[1] = '\0';
    for (i = 0; i < NBLOCK; ++i) {
        for (j = 0; j < BLOCK_CELLS; ++j) {
            _cell = _board.blocks[i].cells[j];
            if (_cell.filled)
                cell_str[0] = '0'+_cell.digit;
            else
                cell_str[0] = '\0';

            ft_set_cur_cell(table, i, j);
            ft_write(table, cell_str);
        }
        if (!(i % TABLE_SIZE))
            ft_add_separator(table);
    }

    /*fflush(stdout);*/
    printf("\b%s\n", ft_to_string(table));
    ft_destroy_table(table);
}

/*void print_board(board _board) {*/
    /*tb_table *table = tb_create(TABLE_WIDTH, TABLE_WIDTH);*/

    /*printf("%s\n", tb_to_str(table, TB_STYLE_DEFAULT));*/

    /*tb_free(table);*/
/*}*/

void print_table_seeder() {
    int i, j;

    for (i = 0; i < NBLOCK; ++i) {
        for (j = 0; j < BLOCK_CELLS; ++j)
            printf("%c", table_seeder[i][j]);
        printf("\n");
    }
}

bool is_valid_board(board _board) {
    /* checks validity of game board by
     * scanning rows, columns and blocks
     * for duplicate entries.
     */
    bool is_valid = true;
    int i, j, r, c, cc, container[NBLOCK];

    /* validate rows */
    for (i = 0; i < NBLOCK; ++i) {
        for (j = 0; j < BLOCK_CELLS; ++j)
            container[j] = _board.blocks[i].cells[j].digit;
        if (!is_sequence(container, BLOCK_CELLS))
            return false;
    }
    /* validate cols */
    for (i = 0; i < NBLOCK; ++i) {
        for (j = 0; j < BLOCK_CELLS; ++j)
            container[j] = _board.blocks[j].cells[i].digit;
        if (!is_sequence(container, BLOCK_CELLS))
            return false;
    }
    /* validate blocks */
    for (r = 0; r < TABLE_SIZE; ++r)
        for (c = 0; c < TABLE_SIZE; ++c) {
            int row_start = r * BLOCK_SIZE;
            int col_start = c * BLOCK_SIZE;
            cc = 0;
            for (i = row_start; i < (row_start+BLOCK_SIZE); ++i)
                for (j = col_start; j < (col_start+BLOCK_SIZE); ++j)
                    container[cc++] = _board.blocks[i].cells[j].digit;
            if (!is_sequence(container, BLOCK_CELLS))
                return false;
        }

    return is_valid;
}

void empty_cells(board *_board) {
    int i, j;
    int filled;

    for (i = 0; i < NBLOCK; ++i)
        for (j = 0; j < BLOCK_CELLS; ++j) {
            filled = rand() & 1;
            _board->blocks[i].cells[j].filled = filled;
        }
}

void destroy_board(board* _board) {
    int i;

    for (i = 0; i < NBLOCK; ++i)
        free(_board->blocks[i].cells);

    free(_board->blocks);
}

void prep_board() {
    init_board(&game_board);
    shuffle_seeder(table_seeder);
    shuffle(token_map, ARR_SIZE(token_map)-1, sizeof(token_map[0]));
    load_from_seed(&game_board);
    empty_cells(&game_board);
}

void handle_cell_write(char *addr, char *digit) {
    assert(strlen(addr) == 2);
    assert(strlen(digit) == 1);

    int r, c, d;
    cell *_cell;

    r = addr[0] - '1';
    c = addr[1] - '1';
    d = digit[0] - '0';

    _cell = &game_board.blocks[r].cells[c];

    if (_cell->filled)
        printf("This cell is already full\n");
    else if (_cell->digit != d)
        printf("Wrong!\n");
    else
        _cell->filled = true;
}

void handle_cell_clear(char *addr) {
    assert(strlen(addr) == 2);

    int r, c;
    cell *_cell;

    r = addr[0] - '1';
    c = addr[1] - '1';

    _cell = &game_board.blocks[r].cells[c];

    if (!_cell->filled)
        printf("This cell is already empty\n");
    else
        _cell->filled = false;
}

void handle_game_restart() {
    destroy_board(&game_board);
    prep_board();
}

void start_game() {
    input user_input;

    prep_board();
    print_board(game_board);

    while ((user_input=parse()).cmd != QUIT) {
        if (!user_input.is_valid) {
            printf("%s\n", user_input.err_message);
            free(user_input.err_message);
            continue;
        }
        switch (user_input.cmd) {
            case WRITE:
                handle_cell_write(user_input.args[0], user_input.args[1]);
                if (board_is_full(game_board))
                    printf("You won!\n");
                else
                    print_board(game_board);
                break;
            case CLEAR:
                handle_cell_clear(user_input.args[0]);
                print_board(game_board);
                break;
            case RESTART:
                handle_game_restart();
                print_board(game_board);
                break;
            default:
                printf("Wrong command!\n");
        }
    }
    destroy_board(&game_board);
}

void usage() {
    printf(
    "Simple sudoku game with CLI. List of commands:\n"
    "     - w <location> <digit>   : Write digit to specified location."
    "E.g. 'w 54 2' write '2' to intersect of 5th row and 4th column in game board.\n"
    "     - c <location>           : Clear digit from location. E.g. 'c 54' "
    "clears digit from intersect of 5th row and 4th column in game board.\n"
    "     - new                    : Starts new game.\n"
    "     - q                      : Quit game.\n"
    );
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usage();
    else
        start_game();

    return 0;
}
