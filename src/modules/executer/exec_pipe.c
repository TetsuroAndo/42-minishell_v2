/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:44:30 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:27:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

static void	pipe_cloexec(int fds[2], t_shell *sh)
{
	xpipe(fds, sh);
	if (fcntl(fds[0], F_SETFD, FD_CLOEXEC) == -1 || fcntl(fds[1], F_SETFD,
			FD_CLOEXEC) == -1)
	{
		perror("fcntl(FD_CLOEXEC)");
		shell_exit(sh, errno);
	}
}

static pid_t	execute_left_pipe(t_ast *node, int fds[2], t_shell *sh)
{
	pid_t	lpid;

	lpid = xfork(sh);
	if (lpid == 0)
	{
		xdup2(&fds[1], STDOUT_FILENO, sh);
		xclose(&fds[0]);
		xclose(&fds[1]);
		exit(exe_run(node->left, sh));
	}
	return (lpid);
}

static pid_t	execute_right_pipe(t_ast *node, int fds[2], t_shell *sh)
{
	pid_t	rpid;

	rpid = xfork(sh);
	if (rpid == 0)
	{
		xdup2(&fds[0], STDIN_FILENO, sh);
		xclose(&fds[0]);
		xclose(&fds[1]);
		exit(exe_run(node->right, sh));
	}
	return (rpid);
}

int	exe_pipe(t_ast *node, t_shell *sh)
{
	int		fds[2];
	pid_t	lpid;
	pid_t	rpid;
	int		sig_held;

	sig_held = 0;
	sig_ignore_parent(&sig_held);
	pipe_cloexec(fds, sh);
	lpid = execute_left_pipe(node, fds, sh);
	rpid = execute_right_pipe(node, fds, sh);
	xclose(&fds[0]);
	xclose(&fds[1]);
	wait_and_status(lpid);
	sig_ignore_parent(&sig_held);
	return (wait_and_status(rpid));
}
