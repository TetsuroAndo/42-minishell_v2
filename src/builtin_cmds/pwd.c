/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:58:14 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:24:59 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__pwd(int argc, char **argv, t_shell *sh)
{
	char	buf[PATH_MAX + 1];

	(void)argv;
	(void)sh;
	if (argc != 1)
		return (ft_dprintf(2, "minishell: pwd: too many arguments\n"), 1);
	if (!getcwd(buf, sizeof buf))
		return (perror("pwd"), 1);
	buf[sizeof buf - 1] = '\0';
	return (printf("%s\n", buf), 0);
}
