/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:40:50 by teando            #+#    #+#             */
/*   Updated: 2025/04/15 19:50:58 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_sem.h"

//wordを引数にコネコネする関数→ft_lstiterみたいなやつで全部に適用したら良さそうかも
// 文字リテラル
// → リダイレクト
// → CMD 解決
//   → ビルドイン
//   → PATH 解決
//     → minishell: %s: command not found
// → 展開可能文字 * $

int is_backslush(char *s)
{
	if (s[0] == '\\')
		return (1);
	return (0);
}

int proc_argv(t_lexical_token *data, int count, t_shell *shell)
{
	// 文字リテラル

	// CMD 解決
	return (0);
}

int proc_redr(t_lexical_token *data, int count, t_shell *shell)
{
	// 文字リテラル

	// リダイレクト
	return (0);
}

int	ast2cmds(t_ast *ast, t_shell *shell)
{
	if (ast == NULL)
		return (1);
	if (ast->ntype != NT_CMD)
	{
		ast2cmds(ast->left, shell);
		ast2cmds(ast->right, shell);
	}
	else
	{
		ms_lstiter(ast->args->argv, proc_argv, shell);// NULLは適用するコマンドを入れるところまだ未実装
		ms_lstiter(ast->args->redr, proc_redr, shell);// NULLは適用するコマンドを入れるところまだ未実装
	}
	return (0);
}

t_status	mod_sem(t_shell *shell)
{
	t_ast *ast;

	ast = shell->ast;

	return (E_NONE);
}
