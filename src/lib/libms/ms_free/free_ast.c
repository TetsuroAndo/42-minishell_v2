/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:56:24 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/11 21:03:58 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"
#include "ms_ast.h"

/**
 * @brief リスト内のデータを解放する関数
 *
 * @param data 解放するデータポインタ
 */
static void	clear_list_item(void *data)
{
	if (data)
		xfree(&data);
}

/**
 * @brief t_args構造体の内容を解放する関数
 *
 * @param args 解放するt_args構造体ポインタ
 */
static void	clear_args(t_args *args)
{
	int	i;

	if (!args)
		return ;
	if (args->cargv)
	{
		i = 0;
		while (args->cargv[i])
		{
			xfree((void **)&args->cargv[i]);
			i++;
		}
		xfree((void **)&args->cargv);
	}
	if (args->argv)
		ft_lstclear(&args->argv, clear_list_item);
	if (args->redr)
		ft_lstclear(&args->redr, free_token);
	if (args->fds[0] > 2)
		xclose(args->fds[0]);
	if (args->fds[1] > 2)
		xclose(args->fds[1]);
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
