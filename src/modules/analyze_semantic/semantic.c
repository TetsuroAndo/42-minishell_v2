/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:18:15 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 23:37:59 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

static int	process_command_args(t_args *args, t_shell *shell)
{
	int	status;

	status = ms_lstiter(args->argv, proc_env, shell);
	if (status != E_NONE)
		return (status);
	del_nul_node(&args->argv);
	status = ms_lstiter(args->argv, proc_split, shell);
	if (status != E_NONE)
		return (status);
	status = ms_lstiter(args->argv, proc_wildcard, shell);
	if (status != E_NONE)
		return (status);
	status = ms_lstiter(args->argv, proc_split, shell);
	if (status != E_NONE)
		return (status);
	status = ms_lstiter(args->argv, proc_quote, shell);
	if (status != E_NONE)
		return (status);
	return (E_NONE);
}

static int	process_redirections(t_args *args, t_shell *shell)
{
	int	status;

	status = ms_lstiter(args->redr, proc_env, shell);
	if (status != E_NONE)
		return (status);
	status = ms_lstiter(args->redr, proc_wildcard, shell);
	if (status != E_NONE)
		return (status);
	status = ms_lstiter(args->redr, proc_quote, shell);
	if (status != E_NONE)
		return (status);
	return (E_NONE);
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
		if (status != E_NONE)
			return (status);
		status = ast2cmds(ast->right, shell);
		return (status);
	}
	status = process_command_args(ast->args, shell);
	if (status != E_NONE)
		return (status);
	status = process_redirections(ast->args, shell);
	if (status != E_NONE)
		return (status);
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
