/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:40:08 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/28 22:37:34 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

int	exe_run(t_ast *node, t_shell *sh)
{
	if (!node)
		return (0);
	else if (node->ntype == NT_CMD)
		return (exe_cmd(node, sh));
	else if (node->ntype == NT_PIPE)
		return (exe_pipe(node, sh));
	else if (node->ntype == NT_AND || node->ntype == NT_OR
		|| node->ntype == NT_LIST)
		return (exe_bool(node, sh));
	else if (node->ntype == NT_SUBSHELL)
		return (exe_sub(node, sh));
	else
		return (1);
}

t_status	mod_exec(t_shell *sh)
{
	int	st;

	if (!sh || !sh->ast)
		return (E_NONE);
	st = exe_run(sh->ast, sh);
	sh->status = st;
	if (sh->debug & DEBUG_EXEC)
	{
		if (st != 0)
			printf("mod_exec Error: %d\n", st);
	}
	return (E_NONE);
}
