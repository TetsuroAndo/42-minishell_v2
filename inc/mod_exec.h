#ifndef MOD_EXEC_H
# define MOD_EXEC_H

# include "core.h"

typedef struct s_lexical_token	t_lexical_token;
typedef struct s_args			t_args;
typedef struct s_ast			t_ast;

// dispatcher
int								builtin_launch(char **argv, t_shell *sh);

// argv utils
char							**toklist_to_argv(t_list *lst, t_shell *sh);

t_status						mod_exec(t_shell *shell);

#endif
