/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:42:27 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:42:57 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

void	fdbackup_enter(t_fdbackup *bk, int tgt, t_shell *sh)
{
	bk->target_fd = tgt;
	bk->saved_fd = xdup(tgt, sh);
}

void	fdbackup_exit(t_fdbackup *bk)
{
	if (bk->saved_fd != -1)
	{
		dup2(bk->saved_fd, bk->target_fd);
		close(bk->saved_fd);
	}
}

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
