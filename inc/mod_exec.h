#ifndef MOD_EXEC_H
# define MOD_EXEC_H

# include "core.h"
# include "mod_sem.h"

typedef struct s_shell			t_shell;
typedef struct s_lexical_token	t_lexical_token;
typedef struct s_args			t_args;
typedef struct s_ast			t_ast;

typedef struct s_fdbackup
{
	int							saved_fd;
	int							target_fd;
}								t_fdbackup;

/* main */
t_status						mod_exec(t_shell *sh);
int								exe_run(t_ast *node, t_shell *sh);
int								exe_cmd(t_ast *node, t_shell *sh);
int								exe_pipe(t_ast *node, t_shell *sh);
int								exe_bool(t_ast *node, t_shell *sh);
int								exe_sub(t_ast *node, t_shell *sh);
int								handle_redr(t_args *args, t_shell *sh);

/* argv utils */
char							**toklist_to_argv(t_list *lst, t_shell *sh);
/* dispatcher */
int								builtin_launch(char **argv, t_shell *sh);

#endif
