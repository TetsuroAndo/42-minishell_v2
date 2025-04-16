/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xstrs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:21:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/16 10:38:18 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

char	**xsplit(char *str, char sep, t_shell *shell)
{
	char	**split;

	split = ft_split(str, sep);
	if (!split)
		shell_exit(shell, E_ALLOCATE);
	return (split);
}
