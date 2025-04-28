/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:49:03 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:41:43 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

static int	heredoc_into_fd(char *body, t_args *args, t_shell *sh)
{
	int	hd[2];

	if (xpipe(hd, sh))
		return (1);
	if (write(hd[1], body, ft_strlen(body)) == -1 || close(hd[1]) == -1)
	{
		close(hd[0]);
		return (1);
	}
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	args->fds[0] = hd[0];
	return (0);
}

static int	handle_input(t_lexical_token *tok, t_args *args, t_shell *sh)
{
	if (tok->type == TT_HEREDOC)
	{
		if (heredoc_into_fd(tok->value, args, sh))
			return (1);
	}
	else if (tok->value && tok->value[0] == '\0')
	{
		if (args->fds[0] > 2)
			xclose(&args->fds[0]);
		args->fds[0] = open("/dev/null", O_RDONLY);
		if (args->fds[0] == -1)
			return (perror("/dev/null"), 1);
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

static int	handle_output(t_lexical_token *tok, t_args *args, t_shell *sh)
{
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (args->fds[1] == -1)
		return (perror(tok->value), 1);
	if (fcntl(args->fds[1], F_SETFD, FD_CLOEXEC) == -1)
	{
		xclose(&args->fds[1]);
		perror("fcntl(FD_CLOEXEC)");
		shell_exit(sh, errno);
	}
	return (0);
}

static int	handle_append(t_lexical_token *tok, t_args *args, t_shell *sh)
{
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	args->fds[1] = open(tok->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (args->fds[1] == -1)
		return (perror(tok->value), 1);
	if (fcntl(args->fds[1], F_SETFD, FD_CLOEXEC) == -1)
	{
		xclose(&args->fds[1]);
		perror("fcntl(FD_CLOEXEC)");
		shell_exit(sh, errno);
	}
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
		result = proc_redr_errs(tok, sh);
		if (result)
			return (result);
		if (tok->type == TT_REDIR_IN || tok->type == TT_HEREDOC)
			result = handle_input(tok, args, sh);
		else if (tok->type == TT_REDIR_OUT)
			result = handle_output(tok, args, sh);
		else if (tok->type == TT_APPEND)
			result = handle_append(tok, args, sh);
		else
			result = 0;
		if (result)
			return (1);
		lst = lst->next;
	}
	return (0);
}
