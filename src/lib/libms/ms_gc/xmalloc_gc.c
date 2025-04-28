/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc_gc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:53:41 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:06:47 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	*xmalloc_gcline(size_t size, t_shell *shell)
{
	void	*p;

	p = ft_gc_malloc(shell->gcli, size);
	if (!p)
		shell_exit(shell, E_SYSTEM);
	return (p);
}
