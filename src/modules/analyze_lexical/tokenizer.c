/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 07:27:28 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_lex.h"

/**
 * @brief 入力行内の記号とその引数の検証を行う
 *
 * @param sh 現在のシステム情報を保持するt_shell構造体へのポインタ
 * @return 検証に成功した場合は1、エラーが発生した場合は0
 */
// static int	validate_input(t_shell *sh)
// {
// 	const char	*line;
// 	size_t		pos;

// 	if (!sh || !sh->source_line)
// 		return (0);
// 	line = sh->source_line;
// 	pos = 0;
// 	while (line[pos])
// 	{
// 		if (validate_special_chars(line, &pos))
// 		{
// 			sh->status = E_SYNTAX;
// 			return (0);
// 		}
// 		if (validate_redirect_missing_arg(line, &pos))
// 		{
// 			sh->status = E_SYNTAX;
// 			return (0);
// 		}
// 		pos++;
// 	}
// 	return (1);
// }

/**
 * @brief 括弧の数のチェックを行う
 *
 * @param sh 現在のシステム情報を保持するt_shell構造体へのポインタ
 * @return 括弧が閉じられていれば1 そうでなければ0
 */
static int	count_paren(t_shell *sh)
{
	t_list			*tokens;
	t_lexical_token	*token;
	size_t			l;
	size_t			r;

	l = 0;
	r = 0;
	tokens = sh->token_list;
	while (tokens->next)
	{
		token = (t_lexical_token *)tokens->data;
		if (token->type == TT_LPAREN)
			l++;
		if (token->type == TT_RPAREN)
			r++;
		tokens = tokens->next;
	}
	return (l == r);
}

/**
 * @brief 入力されたソースラインをトークン化し、その構文を検証する
 *
 * @param sh 現在のシステム情報を保持するt_info構造体へのポインタ
 * @return トークン化および構文検証に成功した場合はE_NONEが返され、
 *         エラーが発生した場合は対応するステータスが返される
 *
 * @details
 * 1. sh->source_lineがNULLでないことを確認する
 * 2. リダイレクト記号の検証を行う
 * 3. tokenize_line関数を呼び出してソースラインをトークン化する
 * 4. トークン化に失敗した場合はステータスをE_SYNTAXに設定する
 * 5. 連続した括弧のチェックを行う
 * 6. すべての処理が成功した場合、E_NONEを返す
 */
t_status	mod_lex(t_shell *sh)
{
	sh->token_list = NULL;
	sh->status = E_NONE;
	if (!sh->source_line)
		return (sh->status = E_SYNTAX);
	// if (!validate_input(sh))
	// 	return (sh->status = E_SYNTAX);
	if (!tokenize_line(sh))
		return (sh->status = E_SYNTAX);
	if (!count_paren(sh))
	{
		ft_dprintf(2, ES_TOKEN);
		return (sh->status = E_SYNTAX);
	}
	if (sh->debug & DEBUG_LEX)
		debug_print_token_list(sh->token_list);
	return (E_NONE);
}
