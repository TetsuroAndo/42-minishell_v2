#ifndef MOD_SEM_H
# define MOD_SEM_H

# include "core.h"

typedef struct s_lexical_token	t_lexical_token;
typedef struct s_args			t_args;
typedef struct s_ast			t_ast;

typedef enum e_quote_state
{
	QS_NONE,
	QS_SINGLE,
	QS_DOUBLE,
	QS_BACK,
}								t_quote_state;

typedef struct s_sem
{
	char			*buf;
	t_quote_state	quote_state;
}								t_sem;

#endif
