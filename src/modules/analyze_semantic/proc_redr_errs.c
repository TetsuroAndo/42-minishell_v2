/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_redr_errs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:33:56 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 20:34:50 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	valid_redir(t_lexical_token *d, t_shell *sh)
{
	int	fd;

	(void)sh;
	if (d->type == TT_REDIR_IN && ft_strchr(d->value, '\n'))
		return (0);
	if (d->type == TT_REDIR_IN)
		fd = open(d->value, O_RDONLY);
	else if (d->type == TT_REDIR_OUT)
		fd = open(d->value, O_WRONLY | O_CREAT, 0644);
	else if (d->type == TT_APPEND)
		fd = open(d->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (0);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

int	proc_redr_errs(t_lexical_token *data, t_shell *shell)
{
	if (!data->value)
		return (E_SYSTEM);
	if (!ft_strchr(data->value, '\n'))
		return (E_NONE);
	if (*data->value == '\0' || ft_strchr(data->value, ' '))
		return (ft_dprintf(2, "minishell: ambiguous redirect\n"),
			E_AMBIGUOUS_REDIR);
	if (valid_redir(data, shell))
		return (ft_dprintf(2, ES_PERMISSION, data->value), E_PERMISSION_DENIED);
	return (E_NONE);
}
