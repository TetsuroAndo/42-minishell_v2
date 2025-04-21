/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:58:14 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 22:25:00 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__pwd(int argc, char **argv, t_shell *sh)
{
	(void)argv;
	if (argc != 1)
		return (ft_dprintf(2, "minishell: pwd: too many arguments\n"), 1);
	return (printf("%s\n", sh->cwd), 0);
}
