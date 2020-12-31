#ifndef __PARSER_H
#define __PARSER_H

#include <stdbool.h>

#define STM_WRITE        "w"
#define STM_CLEAR        "c"
#define STM_RESTART      "new"
#define STM_QUIT         "q"

#define WRITE            0
#define CLEAR            1
#define RESTART          2
#define QUIT             3

#define WORD_MAX_LEN     20
#define LINE_MAX_LEN     100
#define ARG_MAXN         2
#define ARG_MAX_LEN      10
#define ERR_MESSAGE_MAX  1024

#define MSG_INVALID_CMD "Invalid command"
#define MSG_INVALID_ARG "Invalid args"

#define WORD_SEP(c) ((c) == ' ' || (c) == '\n' || (c) == '\t')

typedef struct input {
    int cmd;
    char args[ARG_MAXN][ARG_MAX_LEN];
    char *err_message;
    bool is_valid;
} input;

static int fetchword(char*, char*, int);
static int fetchline(char*, int);
static void init_input(input*);
static int fetchargs(input*, char*);
static bool valid_cell_addr(char*);
static bool valid_cell_dgt(char*);
input parse(void);

#endif /* __PARSER_H */
