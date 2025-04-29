/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:42:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:11:07 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	*xmalloc(size_t size, t_shell *shell)
{
	char	*memory;

	memory = malloc(size);
	if (!memory)
	{
		shell_exit(shell, E_SYSTEM);
	}
	ft_bzero(memory, size);
	return ((void *)memory);
}
