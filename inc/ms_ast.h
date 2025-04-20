#ifndef MS_AST_H
# define MS_AST_H

# include "libft.h"
# include <limits.h>

// token type bit mask
# define TM_TYPE 0xFF00
# define TM_REDIR 0x1200
# define TM_CMD 0x1100
# define TM_OP 0x2000 //EOFもこれでマスクできるのに注意されたし
# define TM_PIPE 0x3000
# define TM_PAREN 0x5000

# define SINGLE_QUOTE 0x010
# define DOUBLE_QUOTE 0x020

typedef enum e_ntype
{
	NT_SIMPLE_CMD, //x
	NT_CMD, //o
	NT_PIPE, //o
	NT_LIST, //o
	NT_EOF, //x
	NT_AND, // o
	NT_OR, // o
	NT_SUBSHELL, // o
	NT_REDIRECT // x
}					t_ntype;

typedef struct s_args
{
	t_list *argv; // 引数リスト
	t_list *redr; // リダイレクトリスト
	t_list *b_argv; // 引数リスト（バックアップ）
	t_list *b_redr; // リダイレクトリスト（バックアップ）
	int				fds[2]; // IN[0], OUT[1]
	pid_t			pid;
}					t_args;

/*
** 抽象構文木構造体
*/
typedef struct s_ast
{
	struct s_ast	*left;
	struct s_ast	*right;
	t_ntype			ntype;
	t_args			*args;
}					t_ast;

#endif
