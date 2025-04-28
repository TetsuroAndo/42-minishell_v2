/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:44:30 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:44:35 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

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