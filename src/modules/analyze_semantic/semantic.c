/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:18:15 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 22:24:48 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int			del_nul_node()
{

}

int	proc_env(void)
{
}

int	proc_split(void)
{
}

int	proc_quote(void)
{
}

int	ast2cmds(t_ast *ast, t_shell *shell)
{
	int	status;

	status = 0;
	if (ast == NULL)
		return (0);
	if (ast->ntype != NT_CMD)
	{
		status = ast2cmds(ast->left, shell);
		status = ast2cmds(ast->right, shell);
	}
	else
	{
		status = ms_lstiter(ast->args->argv, (void *)proc_argv, shell);
		del_nul_node(&ast->args->argv);
		status = ms_lstiter(ast->args->redr, (void *)proc_redr, shell);
	}
	return (status);
}

t_status	mod_sem(t_shell *shell, int isinit)
{
	t_ast		*ast;
	t_status	status;

	ast = shell->ast;
	astlst_backup(ast, shell, isinit);
	status = ast2cmds(ast, shell);
	if (status != E_NONE)
	{
		shell->status = status;
		return (status);
	}
	if (shell->debug & DEBUG_SEM)
		debug_print_sem(ast, shell);
	shell->status = 0;
	return (E_NONE);
}
