/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_launcher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:59:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 17:27:18 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

int	builtin_launch(char **argv, t_shell *sh)
{
	static const t_bimapping	builtins[] = {{"cd", __cd}, {"pwd", __pwd},
	{"export", __export}, {"unset", __unset}, {"env", __env}, {"echo",
		__echo}, {"exit", __exit}, {"true", __true}, {NULL, NULL}};
	size_t						i;

	if (!argv || !argv[0])
		return (127);
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(argv[0], builtins[i].name) == 0)
			return (builtins[i].fn(ft_count_strs(argv), argv, sh));
		++i;
	}
	return (127);
}
