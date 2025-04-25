/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fin_sh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:23:34 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 13:00:06 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	put_sh_final(t_shell *sh, int status)
{
	if (!sh)
		return ;
	ft_dprintf(STDERR_FILENO, "[GC LINE NODE]: %d | [GC LINE FREE]: %d\n", ft_lstsize(sh->gcli), ft_gc_free_info(sh->gcli));
	ft_dprintf(STDERR_FILENO, "[SHELL_EXIT_STATUS]: %d\n", status);
}
