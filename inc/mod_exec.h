/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_exec.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 01:44:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:45:47 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int							saved;
	int							target;
}								t_fdbackup;

/* main */
t_status						mod_exec(t_shell *sh);
int								exe_run(t_ast *node, t_shell *sh);
int								exe_cmd(t_ast *node, t_shell *sh);
int								exe_pipe(t_ast *node, t_shell *sh);
int								exe_bool(t_ast *node, t_shell *sh);
int								exe_sub(t_ast *node, t_shell *sh);
int								handle_redr(t_args *args, t_shell *sh);

/* exec utils*/
void							cleanup_redir_fds(t_args *a);
void							fdbackup_enter(t_fdbackup *bk, int tgt,
									t_shell *sh);
int								wait_and_status(pid_t pid);
char							**toklist_to_argv(t_list *lst, t_shell *sh);
void							sig_ignore_parent(int *enabled);

/* dispatcher */
int								builtin_launch(char **argv, t_shell *sh);

#endif
