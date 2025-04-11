#ifndef MS_AST_H
#define MS_AST_H

#include "libft.h"
#include <limits.h>

typedef enum e_token_category
{
    CMD_ARG = 0x1000,
    CONNECT = 0x2000,
    REDIRECT = 0x1200,
    PAREN = 0x500,
    ERROR = 0x000
} t_token_category;

typedef enum e_ntype
{
    NT_CMD,
    NT_PIPE,
    NT_EOF,
    NT_AND,
    NT_OR,
    NT_REDIRECT
} t_ntype;

typedef struct s_args
{
    char path[PATH_MAX];
    char **cargv;
    t_list *argv;   // 引数リスト
    t_list *redr;   // リダイレクトリスト
    int fds[2];
    pid_t pid;
} t_args;

/*
** 抽象構文木構造体
*/
typedef struct s_ast
{
    struct s_ast *left;
    struct s_ast *right;
    t_ntype ntype;
    t_args *args;
} t_ast;

#endif