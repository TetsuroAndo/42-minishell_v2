/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:58 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:26:57 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__exit(int argc, char **argv, t_shell *sh)
{
	long	code;
	char	*end;

	code = sh->status;
	if (argc > 2)
		return (ft_dprintf(2, "minishell: exit: too many arguments\n"), 1);
	if (argc == 2)
	{
		code = ft_strtol(argv[1], &end, 10);
		if (*end)
		{
			ft_dprintf(2, "minishell: exit: numeric argument required\n");
			code = 255;
		}
	}
	printf("exit\n");
	shell_exit(sh, (int)(code & 0xFF));
	return (0);
}
