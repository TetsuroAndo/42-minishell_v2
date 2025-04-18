/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 01:23:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 01:24:51 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__echo(int argc, char **argv, t_shell *sh)
{
	int	i;
	int	newline;

	(void)sh;
	i = 1;
	newline = 1;
	if (i < argc && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		newline = 0;
		++i;
	}
	while (i < argc)
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (i + 1 < argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
		++i;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (E_NONE);
}
