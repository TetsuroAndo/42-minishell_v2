/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:40:02 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 12:42:08 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	put_line_after(t_shell *sh)
{
	if (!sh)
		return ;
	// ft_dprintf(STDERR_FILENO, "\n");
	// init(NULL) check
}

void	put_line_before(t_shell *sh)
{
	if (!sh)
		return ;
	ft_dprintf(STDERR_FILENO, "\n");
	// ft_gc_dump(sh->gcli, STDERR_FILENO);
}

void	put_line_info(t_shell *sh)
{
	ft_dprintf(STDERR_FILENO, "[sh->status]: %d | ", sh->status);
	ft_dprintf(STDERR_FILENO, "[g_signal_status]: %d | ", g_signal_status);
	// env_spc[?] status
	ft_dprintf(STDERR_FILENO, "[EXIT]: %s\n", sh->env_spc['?']);
	// cwd
	ft_dprintf(STDERR_FILENO, "%s\n", sh->cwd);
}
