/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:58 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:01:13 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__exit(int argc, char **argv, t_shell *sh)
{
	long long	code;
	char		*end;

	code = (long long)ft_atoi(sh->env_spc['?']);
	if (sh->interactive)
		ft_dprintf(STDERR_FILENO, "exit\n");
	if (argc >= 2)
	{
		errno = 0;
		code = ft_strtoll(argv[1], &end, 10);
		if (end != argv[1] + ft_strlen(argv[1]) || errno == ERANGE)
		{
			ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
				argv[1]);
			code = E_NUMERIC;
		}
		else if (argc > 2)
			return (ft_dprintf(2, "minishell: exit: too many arguments\n"), 1);
		else if (code > 255 || code < 0)
			code = code % 256;
	}
	shell_exit(sh, (int)(code & 0xFF));
	return (42);
}
