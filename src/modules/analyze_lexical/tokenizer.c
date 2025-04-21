/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 16:45:38 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_lex.h"

/**
 * @brief 入力されたソースラインをトークン化し、その構文を検証する
 *
 * @param shell 現在のシステム情報を保持するt_info構造体へのポインタ
 * @return トークン化および構文検証に成功した場合はE_NONEが返され、
 *         エラーが発生した場合は対応するステータスが返される
 *
 * @details
 * 1. shell->source_lineがNULLでないことを確認する
 * 2. tokenize_line関数を呼び出してソースラインをトークン化する
 * 3. トークン化に失敗した場合はステータスをE_SYNTAXに設定する
 * 4. validate_syntax関数を呼び出して、得られたトークンの構文を検証する
 * 5. すべての処理が成功した場合、E_NONEを返す
 */
t_status	mod_lex(t_shell *shell)
{
	shell->token_list = NULL;
	shell->status = E_NONE;
	if (!shell->source_line)
		return (E_SYNTAX);
	if (ft_strchr(shell->source_line, '\\') || !tokenize_line(shell))
		return (E_SYNTAX);
	// TODO: 連続したリダイレクト系やnewline表記のバリデーションを行う
	if (shell->debug & DEBUG_LEX)
		debug_print_token_list(shell->token_list);
	return (shell->status);
}
