/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:40:02 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:16:21 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	put_line_after(t_shell *sh)
{
	if (!sh)
		return ;
}

void	put_line_before(t_shell *sh)
{
	if (!sh)
		return ;
	ft_dprintf(STDERR_FILENO, "\n");
}

void	put_line_info(t_shell *sh)
{
	ft_dprintf(STDERR_FILENO, "[sh->status]: %d | ", sh->status);
	ft_dprintf(STDERR_FILENO, "[g_signal_status]: %d | ", g_signal_status);
	ft_dprintf(STDERR_FILENO, "[EXIT]: %s\n", sh->env_spc['?']);
	ft_dprintf(STDERR_FILENO, "%s\n", sh->cwd);
}
