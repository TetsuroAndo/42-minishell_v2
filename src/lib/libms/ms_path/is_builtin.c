/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:20:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:08:36 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	is_builtin(char *cmd)
{
	static const char	*tbl[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", "true", NULL};
	size_t				i;

	i = 0;
	while (tbl[i])
	{
		if (!ft_strcmp(cmd, tbl[i++]))
			return (1);
	}
	return (0);
}
