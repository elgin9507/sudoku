#include <stdlib.h>
#include <string.h>

#include "render.h"

tb_style TB_STYLE_DEFAULT = {
    /*.borders = "┏┓┗┛━┃━┃─│┳┫┣┻╋┼",*/
    .borders =   "1234567890abcdef",
    .cell_marginy = 1,
    .cell_marginx = 1,
};

void tb_set_cur_cell(tb_table *table, int x, int y) {
    table->cx = x;
    table->cy = y;
}

tb_table *tb_create(int width, int height) {
    tb_table *table;
    char *data;

    table = malloc(sizeof(tb_table));
    data = malloc(sizeof(char*) * width * height);

    table->data = data;
    tb_set_cur_cell(table, 0, 0);
    table->width = width;
    table->height = height;

    return table;
}

/*void tb_write(tb_table *table, char *value) {*/
    /*int row = table->cx;*/
    /*int col = table->cy;*/
    /*int w = table->width;*/

    /*strcpy((char*)table->data[row*w+col], value);*/
/*}*/

static int tb_write_top_borders(char *cursor, tb_style style, int ndigit) {
    int i, j, k, nchars = 0;

    *(cursor++) = style.borders[BC_TL_CORNER];
    nchars++;

    for (i = 0; i < ndigit; ++i, ++nchars) {
        for (j = 0; j < style.cell_marginx; ++j, ++nchars)
            *(cursor++) = style.borders[BC_BORDERX];
        *(cursor++) = style.borders[BC_BORDERX];
        for (j = 0; j < style.cell_marginx; ++j, ++nchars)
            *(cursor++) = style.borders[BC_BORDERX];
    }

    *(cursor++) = style.borders[BC_TR_CORNER];
    nchars++;

    return nchars;
}

char *tb_to_str(tb_table *table, tb_style style) {
    char *table_str, *cursor;
    int total_chs, width_chs, height_chs;

    /* calculate count of characters table gonna hold */
    width_chs = \
        table->width // actual data
      + table->width*style.cell_marginx*2 // margins
      + table->width+1 // borders
      + table->width; // new lines
    height_chs =
        table->height  // actual data
      + table->height*style.cell_marginy*2  // margins
      + table->height+1; // borders
    total_chs = width_chs * height_chs;

    table_str = malloc(sizeof(char) * total_chs);
    cursor = table_str;

    cursor += tb_write_top_borders(cursor, style, table->width);
    *cursor = '\0';

    return table_str;
}

void tb_free(tb_table *table) {
    free(table->data);
    free(table);
}
