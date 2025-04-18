/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:37:19 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

static int	_validate_option(char *arg)
{
	if (arg[0] != '-' || arg[1] == '\0')
		return (0);
	while (arg[1])
	{
		if (arg[1] != 'n')
			return (0);
		arg++;
	}
	return (1);
}

t_status	__echo(int argc, char **argv, t_shell *sh)
{
	int	i;
	int	newline;

	(void)sh;
	i = 0;
	newline = 1;
	while (i < argc)
	{
		if (_validate_option(argv[i]))
		{
			newline = 0;
			++i;
			continue ;
		}
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (i + 1 < argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
		++i;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (E_NONE);
}
