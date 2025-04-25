/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fin_sh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:23:34 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 12:47:46 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	put_sh_final(t_shell *sh)
{
	if (!sh)
		return ;
	ft_dprintf(STDERR_FILENO, "\n");
	ft_dprintf(STDERR_FILENO, "[GC LINE NODE]: %d | [GC LINE FREE]: %d\n", ft_lstsize(sh->gcli), ft_gc_free_info(sh->gcli));
}
