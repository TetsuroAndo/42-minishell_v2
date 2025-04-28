/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:18:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:09:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static int	check_permissions(int *c, char *in)
{
	if (access(in, F_OK) != 0)
	{
		*c = E_NO_SUCH_FOD;
		return (ft_dprintf(2, ES_NO_SUCH_FOD, in), 1);
	}
	if (access(in, X_OK) != 0)
	{
		*c = E_PERMISSION_DENIED;
		return (ft_dprintf(2, ES_PERMISSION, in), 1);
	}
	*c = 0;
	return (1);
}

static int	check(int *c, char *in, t_shell *sh)
{
	struct stat	st;

	(void)sh;
	if (is_builtin(in))
	{
		*c = 0;
		return (1);
	}
	else if (ft_strcmp(in, "\0") == 0)
	{
		*c = E_COMMAND_NOT_FOUND;
		return (ft_dprintf(2, ES_CMD_NOT_FOUND, "''"), 1);
	}
	if (stat(in, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			*c = E_IS_DIR;
			return (ft_dprintf(2, ES_IS_DIR, in), 1);
		}
		if (ft_strchr(in, '/'))
			return (check_permissions(c, in));
	}
	return (0);
}

int	path_resolve(char **in, t_shell *sh)
{
	char	**paths;
	char	*test;
	size_t	i;
	int		c;

	if (check(&c, *in, sh))
		return (c);
	paths = xsplit(ms_getenv("PATH", sh), ':', sh);
	if (!paths || !paths[0])
		return (ft_dprintf(2, ES_NO_SUCH_FOD, *in), xfree((void **)&paths),
			E_NO_SUCH_FOD);
	i = 0;
	while (paths[i])
	{
		test = xstrjoin3(paths[i++], "/", *in, sh);
		if (access(test, X_OK) == 0)
		{
			xfree((void **)&*in);
			*in = ms_strdup(test, sh);
			return (xfree((void **)&test), ft_strs_clear(paths), 0);
		}
		xfree((void **)&test);
	}
	return (ft_dprintf(2, ES_CMD_NOT_FOUND, *in), ft_strs_clear(paths), 127);
}
