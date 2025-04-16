/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/16 19:10:32 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

// wordを引数にコネコネする関数→ft_lstiterみたいなやつで全部に適用したら良さそうかも
// 文字リテラル
// → リダイレクト
// → CMD 解決
//   → ビルドイン
//   → PATH 解決
//     → minishell: %s: command not found
// → 展開可能文字 * $

int ft_isbackslush(int c)
{
	return (c == '\\');
}

char	*handle_wildcard(char *in, t_shell *shell)
{
	char *buf;

	return buf;
}

char	*handle_env(char *in, t_shell *shell)
{
	char *buf;

	return buf;
}

int	resolve_path(char *in, t_shell *shell)
{
	return (0);
}

int proc_argv(t_lexical_token *data, int count, t_shell *shell)
{
	// 文字リテラル
	data->value = handle_wildcard(handle_env(data->value,shell),shell);
	if (!data->value)
		return (1);
	// CMD 解決
	if (count == 0)
		if (resolve_path(data->value, shell))
			rteurn (1);
	return (0);
}

int	valid_redir(t_lexical_token *data, t_shell *shell)
{
	return (0);
}

int	proc_redr(t_lexical_token *data, int count, t_shell *shell)
{
	// 文字リテラル
	data->value = handle_wildcard(handle_env(data->value,shell),shell);
	if (!data->value)
		return (1);
	// リダイレクト
	valid_redir(data, shell);
	return (0);
}

int	ast2cmds(t_ast *ast, t_shell *shell)
{
	int	status;

	status = 0;
	if (ast == NULL)
		return (1);
	if (ast->ntype != NT_CMD)
	{
		status += ast2cmds(ast->left, shell);
		status += ast2cmds(ast->right, shell);
	}
	else
	{
		if (ms_lstiter(ast->args->argv, proc_argv, shell))
			return (1);
		if (ms_lstiter(ast->args->redr, proc_redr, shell))
			return (1);
	}
	return ((0 < status));
}

t_status	mod_sem(t_shell *shell)
{
	t_ast	*ast;

	ast = shell->ast;
	if (ast2cmds(ast, shell))
	{
		//エラーハンドリング
		return (shell->status);
	}
	return (E_NONE);
}
