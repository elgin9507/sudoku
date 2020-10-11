#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

static int fetchword(char *word, char *text, int lim) {
    int i, c, spaces;

    spaces = 0;
    while ((c = *text) == ' ') {
        *text++;
        spaces++;
    }

    for (i = 0; i < lim - 1 && (c = *text) != EOF && !WORD_SEP(c); ++i, *word++, *text++)
        *word = c;

    *word++ = '\0';

    return i + spaces;
}


static int fetchline(char *line, int lim) {
    int i, c;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i, *line++)
        *line = c;

    *line++ = '\n';

    return i;
}

static void init_input(input *_input) {
    int i;

    _input->is_valid = true;
    _input->err_message = malloc(sizeof(char) * ERR_MESSAGE_MAX);

    for (i = 0; i < ARG_MAXN; ++i) {
        _input->args[i][0] = '\0';
    }
}

static int fetchargs(input *_input, char *line) {
    int i, nw;
    char *arg;

    arg = malloc(sizeof(char) * WORD_MAX_LEN);

    for (i = 0; i < ARG_MAXN && (nw = fetchword(arg, line, WORD_MAX_LEN)); ++i, line+=nw)
        strcpy(_input->args[i], arg);

    free(arg);

    return i;
}

static bool valid_cell_addr(char *arg) {
    bool is_valid = true;

    if (strlen(arg) != 2)
        return false;

    for (int i = 0; i < 2; ++i)
        if (!(arg[i] <= '9' && arg[i] >= '1'))
            return false;

    return is_valid;
}

static bool valid_cell_dgt(char *arg) {
    bool is_valid = true;

    if (strlen(arg) != 1)
        return false;

    if (!(arg[0] <= '9' && arg[0] >= '1'))
        return false;

    return is_valid;
}

input parse() {
    input inp;
    char *stm, *line, *cursor;

    init_input(&inp);
    line = malloc(sizeof(char) * LINE_MAX_LEN);
    cursor = line;
    stm = malloc(sizeof(char) * WORD_MAX_LEN);

    fetchline(line, LINE_MAX_LEN);
    cursor += fetchword(stm, line, WORD_MAX_LEN);

    if (!strcmp(stm, STM_WRITE))
        inp.cmd = WRITE;
    else if (!strcmp(stm, STM_CLEAR))
        inp.cmd = CLEAR;
    else if(!strcmp(stm, STM_RESTART))
        inp.cmd = RESTART;
    else if (!strcmp(stm, STM_QUIT))
        inp.cmd = QUIT;
    else {
        inp.is_valid = false;
        strcpy(inp.err_message, MSG_INVALID_CMD);
        return inp;
    }

    switch (inp.cmd) {
        case WRITE:
            fetchargs(&inp, cursor);
            if (!valid_cell_addr(inp.args[0]) || !valid_cell_dgt(inp.args[1])) {
                inp.is_valid = false;
                strcpy(inp.err_message, MSG_INVALID_ARG);
            }
            break;
        case CLEAR:
            fetchargs(&inp, cursor);
            if (!valid_cell_addr(inp.args[0])) {
                inp.is_valid = false;
                strcpy(inp.err_message, MSG_INVALID_ARG);
            }
            break;
        case RESTART:
        case QUIT:
            break;
        default:
            strcpy(inp.err_message, MSG_INVALID_ARG);
    }

    free(line);
    free(stm);

    return inp;
}
