/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_ast.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:45:20 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 18:37:09 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_AST_H
# define MS_AST_H

# include "libft.h"
# include <limits.h>

/*token type bit mask*/
# define TM_TYPE 0xFF00
# define TM_REDIR 0x1200
# define TM_CMD 0x1100
# define TM_OP 0x2000
# define TM_PIPE 0x3000
# define TM_PAREN 0x5000

# define SINGLE_QUOTE 0x010
# define DOUBLE_QUOTE 0x020

typedef enum e_ntype
{
	NT_SIMPLE_CMD,
	NT_CMD,
	NT_PIPE,
	NT_LIST,
	NT_EOF,
	NT_AND,
	NT_OR,
	NT_SUBSHELL,
	NT_REDIRECT
}					t_ntype;

typedef struct s_args
{
	t_list			*argv;
	t_list			*redr;
	t_list			*b_argv;
	t_list			*b_redr;
	int				fds[2];
	pid_t			pid;
}					t_args;

typedef struct s_ast
{
	struct s_ast	*left;
	struct s_ast	*right;
	t_ntype			ntype;
	t_args			*args;
}					t_ast;

#endif
