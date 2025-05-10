/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:58:14 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 10:01:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__pwd(int argc, char **argv, t_shell *sh)
{
	(void)argv;
	(void)argc;
	printf("%s\n", sh->cwd);
	return (0);
}
