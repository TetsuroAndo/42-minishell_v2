/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:58 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 21:56:45 by tomsato          ###   ########.fr       */
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
		if (end != argv[1] + ft_strlen(argv[1]) || code > 255 || code < 0)
		{
			ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", argv[1]);
			code = 255;
		}
	}
	if (sh->interactive)
		printf("exit\n");
	shell_exit(sh, (int)(code & 0xFF));
	return (42);
}
