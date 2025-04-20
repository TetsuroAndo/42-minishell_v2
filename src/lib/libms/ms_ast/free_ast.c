/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:56:24 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/21 03:08:33 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"
#include "ms_ast.h"

/**
 * @brief t_args構造体の内容を解放する関数
 *
 * @param args 解放するt_args構造体ポインタ
 */
static void	clear_args(t_args *args)
{
	if (!args)
		return ;
	if (args->argv)
		ft_lstclear(&args->argv, free_token);
	if (args->redr)
		ft_lstclear(&args->redr, free_token);
	if (args->b_argv)
		ft_lstclear(&args->b_argv, free_token);
	if (args->b_redr)
		ft_lstclear(&args->b_redr, free_token);
	if (args->fds[0] > 2)
		xclose(&args->fds[0]);
	if (args->fds[1] > 2)
		xclose(&args->fds[1]);
	if (args->pid != -1)
		waitpid(args->pid, NULL, 0);
	xfree((void **)&args);
}

/**
 * @brief AST（抽象構文木）を再帰的に解放する関数
 *
 * @param ast 解放するAST構造体ポインタ
 */
void	free_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	// 左の子ノードを再帰的に解放
	if ((*ast)->left)
		free_ast(&(*ast)->left);
	// 右の子ノードを再帰的に解放
	if ((*ast)->right)
		free_ast(&(*ast)->right);
	// 引数データの解放
	if ((*ast)->args)
		clear_args((*ast)->args);
	// ノード自体の解放
	xfree((void **)ast);
	*ast = NULL;
}
