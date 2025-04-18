#ifndef MOD_SEM_H
# define MOD_SEM_H

# include "core.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <unistd.h>

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

# define EX_IN 'I'
# define EX_OUT 'O'

typedef struct s_extract
{
	char						*str;
	char						*map;
}								t_extract;

typedef struct s_sem
{
	char						*buf;
	t_quote_state				quote_state;
}								t_sem;

t_status						mod_sem(t_shell *shell);
void							debug_print_sem(t_ast *ast, t_shell *sh);

char							*handle_wildcard(char *in, t_shell *sh);

// helper
t_quote_state					is_quote_type(int c);
int								check_qs(int c, t_sem *sem);

// wildcard_helper
t_extract						*convert_ex(char *str, t_shell *shell);
char							*replace_with_unquoted(char *str_ptr,
									t_shell *shell);

#endif
