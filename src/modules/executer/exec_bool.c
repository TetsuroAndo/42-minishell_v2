/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:46:02 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:28:48 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

static void	update_shell_status(int status, t_shell *sh)
{
	sh->status = status;
	xfree((void **)&sh->env_spc['?']);
	sh->env_spc['?'] = xitoa(status, sh);
	mod_sem(sh, 0);
}

int	exe_bool(t_ast *node, t_shell *sh)
{
	int	st_left;

	if (sh->env_updated)
		mod_sem(sh, 0);
	sh->env_updated = 0;
	st_left = exe_run(node->left, sh);
	update_shell_status(st_left, sh);
	if ((node->ntype == NT_AND && st_left == 0) || (node->ntype == NT_OR
			&& st_left != 0) || (node->ntype == NT_LIST))
	{
		if (sh->env_updated)
			mod_sem(sh, 0);
		sh->env_updated = 0;
		return (exe_run(node->right, sh));
	}
	return (st_left);
}
