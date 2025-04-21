/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:40:02 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 19:38:11 by teando           ###   ########.fr       */
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
	ft_dprintf(STDERR_FILENO, "[sh->status]: %d\n", sh->status);
	ft_dprintf(STDERR_FILENO, "[g_signal_status]: %d\n", g_signal_status);
}

void	put_line_info(t_shell *sh)
{
	ft_dprintf(STDERR_FILENO, "\n");
	// env_spc[?] status
	printf("EXIT[%s] | ", sh->env_spc['?']);
	// cwd
	printf("%s\n", sh->cwd);
}
