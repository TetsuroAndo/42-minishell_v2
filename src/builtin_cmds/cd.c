/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:59:45 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 23:04:46 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "builtin_cmds.h"

/* cd [dir] : dir が NULL のときは $HOME */
t_status	__cd(int argc, char **argv, t_shell *sh)
{
	const char	*dest;
	char		cwd[PATH_MAX + 1];

	if (argc == 1)
		dest = ms_getenv("HOME", sh);
	else if (argc == 2)
		dest = argv[1];
	else
		return (ft_dprintf(2, "minishell: cd: too many arguments\n"), 1);
	if (chdir(dest) == -1)
		return (perror(dest), 1);
	/* 成功したら PWD / OLDPWD を更新 */
	if (getcwd(cwd, sizeof cwd))
	{
		ms_setenv_item("PWD", cwd, sh);
		ms_setenv_item("OLDPWD", ms_getenv("PWD", sh), sh);
	}
	return (0);
}
