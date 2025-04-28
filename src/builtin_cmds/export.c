/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:11:41 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 18:58:19 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"
#include "mod_sem.h"

static int __print_export(char *entry)
{
	char	tmp[PATH_MAX + 1];
	size_t	len;

	len = ft_strcspn(entry, "=");
	ft_strlcpy(tmp, entry, len + 1);
	ft_dprintf(STDOUT_FILENO, "declare -x %s", tmp);
	if (len == ft_strlen(entry))
		return (ft_dprintf(STDOUT_FILENO, "\n"), 1);
	ft_dprintf(STDOUT_FILENO, "=\"%s\"", entry + len + 1);
	return (ft_dprintf(STDOUT_FILENO, "\n"), 0);
}

static t_status	__put_export(t_shell *sh)
{
	char	**envp;
	int		len;
	int		i;
	int		j;

	envp = ft_list_to_strs(sh->env_map);
	len = ft_list_size(sh->env_map);
	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
				ft_swap(&envp[i], &envp[j]);
			j++;
		}
		__print_export(envp[i++]);
	}
	ft_strs_clear(envp);
	return (0);
}

t_status	__export(int argc, char **argv, t_shell *sh)
{
	int	i;

	if (argc == 1)
		return (__put_export(sh));
	i = 0;
	while (++i < argc)
	{
		if (ms_setenv(ms_strdup(argv[i], sh), sh) != E_NONE)
			return (1);
		sh->env_updated = 1;
	}
	return (0);
}
