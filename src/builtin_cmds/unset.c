/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:44:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 02:54:41 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__unset(int argc, char **argv, t_shell *sh)
{
    int i;

	(void)argv;
	(void)sh;
	if (argc == 1)
		return (ft_dprintf(2, "minishell: unset: missing operand\n"), 1);
    i = 0;
	while (++i < argc)
	{
		if (ms_unset(argv[i], sh) != E_NONE)
			return (1);
		sh->env_updated = 1;
	}
	return (E_NONE);
}
