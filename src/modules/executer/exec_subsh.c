/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subsh.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:45:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 00:45:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

int	exe_sub(t_ast *node, t_shell *sh)
{
	pid_t	pid;

	pid = xfork(sh);
	if (pid == 0)
	{
		sh->interactive = 0;
		exit(exe_run(node->left, sh));
	}
	return (wait_and_status(pid));
}
