/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astlst_backup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:10:46 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 19:43:28 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

static void	backup_node_args(t_ast *ast, t_shell *sh, int isinit)
{
	t_args	*ast_args;

	if (!ast || !ast->args)
		return ;
	ast_args = ast->args;
	if (isinit)
	{
		ast_args->b_argv = ms_lstcopy(ast_args->argv, free_token, sh);
		ast_args->b_redr = ms_lstcopy(ast_args->redr, free_token, sh);
	}
	ft_lstclear(&ast_args->argv, free_token);
	ast_args->argv = ms_lstcopy(ast_args->b_argv, free_token, sh);
	ft_lstclear(&ast_args->redr, free_token);
	ast_args->redr = ms_lstcopy(ast_args->b_redr, free_token, sh);
}

void	astlst_backup(t_ast *ast, t_shell *shell, int isinit)
{
	if (!ast)
		return ;
	backup_node_args(ast, shell, isinit);
	if (ast->left)
		astlst_backup(ast->left, shell, isinit);
	if (ast->right)
		astlst_backup(ast->right, shell, isinit);
}
/*
void	backup_node_args(t_ast *ast, t_shell *shell)
{
	t_args	*ast_args;

	if (!ast || !ast->args)
		return ;
	ast_args = ast->args;
	if (ast_args->b_argv == NULL)
		ast_args->b_argv = ms_lstcopy(ast_args->argv, free_token, shell);
	if (ast_args->b_redr == NULL)
	{
		proc_all_heredocs(ast_args->redr, shell);
		ast_args->b_redr = ms_lstcopy(ast_args->redr, free_token, shell);
	}
	ft_lstclear(&ast_args->argv, free_token);
	ft_lstclear(&ast_args->redr, free_token);
	ast_args->argv = ms_lstcopy(ast_args->b_argv, free_token, shell);
	ast_args->redr = ms_lstcopy(ast_args->b_redr, free_token, shell);
}
*/
