/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 01:23:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 22:20:39 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__echo(int argc, char **argv, t_shell *sh)
{
	int	i;
	int	newline;
	int	op_end_flag;

	(void)sh;
	i = 0;
	newline = 1;
	op_end_flag = 0;
	while (++i < argc)
	{
		if (!op_end_flag && argv[i][0] == '-' && ft_strspn(argv[i] + 1,
				"n") == ft_strlen(argv[i] + 1) && argv[i][1])
		{
			newline = 0;
			continue ;
		}
		op_end_flag = 1;
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (i + 1 < argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (E_NONE);
}
