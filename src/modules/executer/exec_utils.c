/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:42:27 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:30:10 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

void	cleanup_redir_fds(t_args *a)
{
	if (a->fds[0] > 2)
	{
		xclose(&a->fds[0]);
	}
	if (a->fds[1] > 2)
	{
		xclose(&a->fds[1]);
	}
}

void	fdbackup_enter(t_fdbackup *bk, int tgt, t_shell *sh)
{
	bk->target = tgt;
	bk->saved = xdup(tgt, sh);
	if (fcntl(bk->saved, F_SETFD, FD_CLOEXEC) == -1)
	{
		xclose(&bk->saved);
		perror("fcntl(FD_CLOEXEC)");
		shell_exit(sh, errno);
	}
}

int	wait_and_status(pid_t pid)
{
	int	wst;

	if (waitpid(pid, &wst, 0) == -1)
		return (E_SYSTEM);
	if (WIFEXITED(wst))
		return (WEXITSTATUS(wst));
	return (128 + WTERMSIG(wst));
}

char	**toklist_to_argv(t_list *lst, t_shell *sh)
{
	t_lexical_token	*tok;
	char			**argv;
	size_t			n;
	size_t			i;

	n = ft_lstsize(lst);
	argv = xmalloc_gcline(sizeof(char *) * (n + 1), sh);
	i = 0;
	while (lst)
	{
		tok = lst->data;
		argv[i++] = tok->value;
		lst = lst->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	sig_ignore_parent(int *enabled)
{
	static struct sigaction	old_int;
	static struct sigaction	old_quit;
	struct sigaction		ign;

	if (*enabled)
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
