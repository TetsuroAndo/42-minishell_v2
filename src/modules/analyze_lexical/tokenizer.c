/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 14:32:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief 入力行内の記号とその引数の検証を行う
 * 
 * @param shell 現在のシステム情報を保持するt_shell構造体へのポインタ
 * @return 検証に成功した場合は1、エラーが発生した場合は0
 */
static int	validate_redirects(t_shell *shell)
{
	const char	*line;
	size_t		pos;

	if (!shell || !shell->source_line)
		return (0);
	line = shell->source_line;
	pos = 0;
	while (line[pos])
	{
		if (validate_special_chars(line, &pos))
		{
			shell->status = E_SYNTAX;
			return (0);
		}
		if (validate_redirect_missing_arg(line, &pos))
		{
			shell->status = E_SYNTAX;
			return (0);
		}
		pos++;
	}
	return (1);
}

/**
 * @brief 入力されたソースラインをトークン化し、その構文を検証する
 *
 * @param shell 現在のシステム情報を保持するt_info構造体へのポインタ
 * @return トークン化および構文検証に成功した場合はE_NONEが返され、
 *         エラーが発生した場合は対応するステータスが返される
 *
 * @details
 * 1. shell->source_lineがNULLでないことを確認する
 * 2. リダイレクト記号の検証を行う
 * 3. tokenize_line関数を呼び出してソースラインをトークン化する
 * 4. トークン化に失敗した場合はステータスをE_SYNTAXに設定する
 * 5. すべての処理が成功した場合、E_NONEを返す
 */
t_status	mod_lex(t_shell *shell)
{
	shell->token_list = NULL;
	shell->status = E_NONE;
	if (!shell->source_line || ft_strchr(shell->source_line, '\\'))
		return (E_SYNTAX);
	if (!validate_redirects(shell))
		return (E_SYNTAX);
	if (!tokenize_line(shell))
		return (E_SYNTAX);
	if (shell->debug & DEBUG_LEX)
		debug_print_token_list(shell->token_list);
	return (E_NONE);
}
