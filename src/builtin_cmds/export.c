/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:11:41 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:26:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

static t_status	put_export(t_shell *sh)
{
	char	**envp;
	int		len;
	int		i;
	int		j;

	envp = ft_list_to_strs(sh->env_map);
	len = ft_list_size(sh->env_map);
	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
				ft_swap(&envp[i], &envp[j]);
			j++;
		}
		printf("%s\n", envp[i++]);
	}
	printf("%s\n", envp[i]);
	ft_strs_clear(envp);
	return (0);
}

t_status	__export(int argc, char **argv, t_shell *sh)
{
	int	i;

	if (argc == 1)
		return (put_export(sh));
	i = 0;
	while (++i < argc)
	{
		if (ms_setenv(ft_strdup(argv[i]), sh) != E_NONE)
			return (1);
	}
	return (0);
}
