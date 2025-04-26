#ifndef MOD_SEM_H
# define MOD_SEM_H

# include "core.h"
# include "ms_readline.h"
# include <dirent.h>
# include <fcntl.h>
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

typedef struct s_expenv
{
	char						*buf;
	t_quote_state				quote_state;
}								t_expenv;

t_status						mod_sem(t_shell *shell, int isinit);
void							astlst_backup(t_ast *ast, t_shell *shell,
									int isinit);
int								proc_wildcard(t_list **lst, int index,
									t_shell *sh);
t_extract						*convert_ex(char *str, t_shell *shell);
void							free_exstract(t_extract *ex, t_shell *shell);
int								proc_split(t_list **lst, int index,
									t_shell *sh);
void							del_nul_node(t_list **list);
#endif
