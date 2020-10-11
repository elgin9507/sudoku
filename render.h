#ifndef __RENDER_H
#define __RENDER_H

/* border chars indices definitions */
#define BC_TL_CORNER        0
#define BC_TR_CORNER        1
#define BC_BL_CORNER        2
#define BC_BR_CORNER        3
#define BC_BORDERX          4
#define BC_BORDERY          5
#define BC_BLOCK_BORDERX    6
#define BC_BLOCK_BORDERY    7
#define BC_CELL_BORDERX     8
#define BC_CELL_BORDERY     9
#define BC_INS_OUTERT      10
#define BC_INS_OUTERR      11
#define BC_INS_OUTERB      12
#define BC_INS_OUTERL      13
#define BC_INS_BLOCK       14
#define BC_INS_CELL        15
/* --------------------------------------- */

typedef struct tb_style {
    /* game table border style */
    char *borders;
    unsigned short cell_marginy;
    unsigned short cell_marginx;
} tb_style;

typedef struct tb_table {
    char *data;
    tb_style style;
    int cx; /* current x */
    int cy; /* current y */
    int width;
    int height;
} tb_table;

extern tb_style TB_STYLE_DEFAULT;

static int tb_write_top_borders(char*, tb_style, int);
/* API */
void tb_set_cur_cell(tb_table*, int, int);
tb_table *tb_create(int, int);
void tb_write(tb_table*, char*);
char *tb_to_str(tb_table*, tb_style);
void tb_free(tb_table*);
/* --------------------------------------- */

#endif /* __RENDER_H */
