/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:33:11 by teando            #+#    #+#             */
/*   Updated: 2025/04/23 16:53:22 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

/* ─────────── fd backup ─────────── */
static void	fdbackup_enter(t_fdbackup *bk, int tgt, t_shell *sh)
{
	bk->target_fd = tgt;
	bk->saved_fd = xdup(tgt, sh);
}

static void	fdbackupexit(t_fdbackup *bk)
{
	if (bk->saved_fd != -1)
	{
		dup2(bk->saved_fd, bk->target_fd);
		close(bk->saved_fd);
	}
}

/* ──────── signal handling ──────── */
void	sig_ignore_parent(int *enabled)
{
	static struct sigaction	old_int;
	static struct sigaction	old_quit;
	struct sigaction		ign;

	if (*enabled) /* restore */
	{
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		*enabled = 0;
		return ;
	}
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	*enabled = 1;
}

/* ========================================================= */
/*                   AST dispatcher : exe_run                */
/* ========================================================= */

int	exe_run(t_ast *node, t_shell *sh)
{
	if (!node)
		return (0);
	else if (node->ntype == NT_CMD)
		return (exe_cmd(node, sh));
	else if (node->ntype == NT_PIPE)
		return (exe_pipe(node, sh));
	else if (node->ntype == NT_AND || node->ntype == NT_OR
		|| node->ntype == NT_LIST)
		return (exe_bool(node, sh));
	else if (node->ntype == NT_SUBSHELL)
		return (exe_sub(node, sh));
	else
		return (1);
}

/* ========================================================= */
/*                        NT_CMD                             */
/* ========================================================= */

static int	prepare_cmd_args(t_ast *node, char ***argv, int *flag, t_shell *sh)
{
	struct stat	sb;

	*flag = 0;
	if (handle_redr(node->args, sh))
		return (1);
	*argv = toklist_to_argv(node->args->argv, sh);
	if (!*argv)
	{
		*flag = 1;
		return (E_SYSTEM);
	}
	if (!(*argv)[0])
	{
		*flag = 1;
		return (E_NONE);
	}
	return (E_NONE);
	if (stat((*argv)[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		return (E_IS_DIR);
	return (0);
}

static void	setup_redirections(t_ast *node, t_fdbackup *bk_in,
		t_fdbackup *bk_out, t_shell *sh)
{
	*bk_in = (t_fdbackup){-1, STDIN_FILENO};
	*bk_out = (t_fdbackup){-1, STDOUT_FILENO};
	if (node->args->fds[0] != -1)
	{
		fdbackup_enter(bk_in, STDIN_FILENO, sh);
		xdup2(&node->args->fds[0], STDIN_FILENO, sh);
	}
	if (node->args->fds[1] != -1)
	{
		fdbackup_enter(bk_out, STDOUT_FILENO, sh);
		xdup2(&node->args->fds[1], STDOUT_FILENO, sh);
	}
}

static int	execute_external_cmd(char **argv, t_ast *node, t_shell *sh)
{
	int		wstatus;
	int		sig_held;
	char	**env;

	sig_held = 0;
	sig_ignore_parent(&sig_held);
	node->args->pid = xfork(sh);
	if (node->args->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		env = ft_list_to_strs(sh->env_map);
		if (!*env)
			return (ft_strs_clear(env), E_SYSTEM);
		if (!(*env)[0])
			return (ft_strs_clear(env), E_NONE);
		execve(argv[0], argv, env);
		perror(argv[0]);
		exit(127);
	}
	waitpid(node->args->pid, &wstatus, 0);
	sig_ignore_parent(&sig_held);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (128 + WTERMSIG(wstatus));
}

int	exe_cmd(t_ast *node, t_shell *sh)
{
	char		**argv;
	t_fdbackup	bk_in;
	t_fdbackup	bk_out;
	int			status;
	int			flag;

	flag = 0;
	argv = NULL;
	if (!node || node->ntype != NT_CMD)
		return (1);
	status = prepare_cmd_args(node, &argv, &flag, sh);
	if (flag)
		return (status);
	setup_redirections(node, &bk_in, &bk_out, sh);
	if (is_builtin(argv[0]))
		status = builtin_launch(argv, sh);
	else
		status = execute_external_cmd(argv, node, sh);
	fdbackupexit(&bk_in);
	fdbackupexit(&bk_out);
	return (status);
}

/* ========================================================= */
/*                         NT_PIPE                           */
/* ========================================================= */

static pid_t	execute_left_pipe(t_ast *node, int fds[2], t_shell *sh)
{
	pid_t	lpid;
	int		st;

	lpid = xfork(sh);
	if (lpid == 0)
	{
		xdup2(&fds[1], STDOUT_FILENO, sh);
		xclose(&fds[0]);
		xclose(&fds[1]);
		st = exe_run(node->left, sh);
		exit(st);
	}
	return (lpid);
}

static pid_t	execute_right_pipe(t_ast *node, int fds[2], t_shell *sh)
{
	pid_t	rpid;
	int		st;

	rpid = xfork(sh);
	if (rpid == 0)
	{
		xdup2(&fds[0], STDIN_FILENO, sh);
		xclose(&fds[0]);
		xclose(&fds[1]);
		st = exe_run(node->right, sh);
		exit(st);
	}
	return (rpid);
}

static int	wait_for_pipe_children(pid_t lpid, pid_t rpid, int *sig_held)
{
	int	st_l;
	int	st_r;

	waitpid(lpid, &st_l, 0);
	waitpid(rpid, &st_r, 0);
	sig_ignore_parent(sig_held);
	if (WIFEXITED(st_r))
		return (WEXITSTATUS(st_r));
	return (128 + WTERMSIG(st_r));
}

int	exe_pipe(t_ast *node, t_shell *sh)
{
	int		fds[2];
	pid_t	lpid;
	pid_t	rpid;
	int		sig_held;
	int		status;

	sig_held = 0;
	sig_ignore_parent(&sig_held);
	xpipe(fds, sh);
	lpid = execute_left_pipe(node, fds, sh);
	rpid = execute_right_pipe(node, fds, sh);
	xclose(&fds[0]);
	xclose(&fds[1]);
	status = wait_for_pipe_children(lpid, rpid, &sig_held);
	return (status);
}

/* ========================================================= */
/*                  NT_AND / NT_OR / NT_LIST                  */
/* ========================================================= */

static void	update_shell_status(int status, t_shell *sh)
{
	sh->status = status;
	xfree((void **)&sh->env_spc['?']);
	sh->env_spc['?'] = xitoa(status, sh);
	mod_sem(sh);
}

int	exe_bool(t_ast *node, t_shell *sh)
{
	int	st_left;
	int	run_right;

	if (sh->env_updated)
	{
		mod_sem(sh);
		sh->env_updated = 0;
	}
	st_left = exe_run(node->left, sh);
	update_shell_status(st_left, sh);
	run_right = 0;
	if (node->ntype == NT_AND && st_left == 0)
		run_right = 1;
	if (node->ntype == NT_OR && st_left != 0)
		run_right = 1;
	if (node->ntype == NT_LIST)
		run_right = 1;
	if (run_right)
	{
		mod_sem(sh);
		sh->env_updated = 0;
		return (exe_run(node->right, sh));
	}
	return (st_left);
}

/* ========================================================= */
/*                        NT_SUBSHELL                        */
/* ========================================================= */

int	exe_sub(t_ast *node, t_shell *sh)
{
	pid_t	pid;
	int		wst;
	int		st;

	pid = xfork(sh);
	if (pid == 0)
	{
		sh->interactive = 0;
		st = exe_run(node->left, sh); /* subshell の AST は left に格納 */
		exit(st);
	}
	waitpid(pid, &wst, 0);
	if (WIFEXITED(wst))
		return (WEXITSTATUS(wst));
	return (128 + WTERMSIG(wst));
}

/* ========================================================= */
/*                      ヒアドキュメント                        */
/* ========================================================= */
int	heredoc_into_fd(char *body, t_args *args, t_shell *sh)
{
	int	hd[2];

	if (xpipe(hd, sh))
		return (1);
	/* write() は短い本文なら一括送信で十分。失敗時はシェルごと死ぬ想定。*/
	if (write(hd[1], body, ft_strlen(body)) == -1 || close(hd[1]) == -1)
	{
		perror("heredoc write");
		close(hd[0]);
		return (1);
	}
	/* 直近の < や << が勝つ POSIX 仕様を踏襲。 */
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	args->fds[0] = hd[0]; /* read end を stdin 置換候補に */
	return (0);
}

/* ========================================================= */
/*                    redirect 一括ハンドラ                    */
/* ========================================================= */

static int	handle_input_redirection(t_lexical_token *tok, t_args *args,
		t_shell *sh)
{
	if (tok->value && ft_strchr(tok->value, '\n'))
	{
		if (heredoc_into_fd(tok->value, args, sh))
			return (1);
	}
	else
	{
		if (args->fds[0] > 2)
			xclose(&args->fds[0]);
		args->fds[0] = open(tok->value, O_RDONLY);
		if (args->fds[0] == -1)
			return (perror(tok->value), 1);
	}
	return (0);
}

static int	handle_output_redirection(t_lexical_token *tok, t_args *args)
{
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (args->fds[1] == -1)
		return (perror(tok->value), 1);
	return (0);
}

static int	handle_append_redirection(t_lexical_token *tok, t_args *args)
{
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (args->fds[1] == -1)
		return (perror(tok->value), 1);
	return (0);
}

int	handle_redr(t_args *args, t_shell *sh)
{
	t_list			*lst;
	t_lexical_token	*tok;
	int				result;

	lst = args->redr;
	while (lst)
	{
		tok = lst->data;
		if (tok->type == TT_REDIR_IN)
			result = handle_input_redirection(tok, args, sh);
		else if (tok->type == TT_REDIR_OUT)
			result = handle_output_redirection(tok, args);
		else if (tok->type == TT_APPEND)
			result = handle_append_redirection(tok, args);
		else
			result = 0;
		if (result)
			return (1);
		lst = lst->next;
	}
	return (0);
}
