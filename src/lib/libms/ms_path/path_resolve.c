/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:18:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 16:21:24 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	path_resolve(char **in, t_shell *sh)
{
	char	**paths;
	char	*test;
	size_t	i;

	if (is_builtin(*in))
		return (0);
	if (ft_strchr(*in, '/'))
		return (access(*in, X_OK));
	paths = xsplit(ms_getenv("PATH", sh), ':', sh);
	if (!paths)
		return (1);
	i = 0;
	while (paths[i])
	{
		test = xstrjoin3(paths[i++], "/", *in, sh);
		if (access(test, X_OK) == 0)
		{
			free(*in);
			*in = ms_strdup(test, sh);
			return (ft_strs_clear(paths), 0);
		}
		free(test);
	}
	ft_dprintf(2, "minishell: %s: command not found\n", *in);
	return (ft_strs_clear(paths), 1);
}
