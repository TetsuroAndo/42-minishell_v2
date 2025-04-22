/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 18:03:26 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief 入力行内の記号とその引数の検証を行う
 *
 * @param sh 現在のシステム情報を保持するt_shell構造体へのポインタ
 * @return 検証に成功した場合は1、エラーが発生した場合は0
 */
static int	validate_input(t_shell *sh)
{
	const char	*line;
	size_t		pos;

	if (!sh || !sh->source_line)
		return (0);
	line = sh->source_line;
	pos = 0;
	while (line[pos])
	{
		if (validate_special_chars(line, &pos))
		{
			sh->status = E_SYNTAX;
			return (0);
		}
		if (validate_redirect_missing_arg(line, &pos))
		{
			sh->status = E_SYNTAX;
			return (0);
		}
		pos++;
	}
	return (1);
}

/**
 * @brief 連続した括弧のチェックを行う
 * TT_LPARENの次にTT_LPARENまたはTT_RPARENが来た場合はエラーとする
 *
 * @param sh 現在のシステム情報を保持するt_shell構造体へのポインタ
 * @return エラーがある場合は1、ない場合は0
 */
static int	check_token(t_shell *sh)
{
	t_list			*current;
	t_lexical_token	*token;
	t_lexical_token	*next_token;

	if (!sh || !sh->token_list)
		return (0);
	current = sh->token_list;
	while (current && current->next)
	{
		token = (t_lexical_token *)current->data;
		next_token = (t_lexical_token *)current->next->data;
		if (token->type == TT_LPAREN && (next_token->type == TT_LPAREN
				|| next_token->type == TT_RPAREN))
		{
			if (next_token->type == TT_LPAREN)
				ft_putendl_fd("syntax error near unexpected token `(`",
					STDERR_FILENO);
			else
				ft_putendl_fd("syntax error near unexpected token `)`",
					STDERR_FILENO);
			return (sh->status = E_SYNTAX);
		}
		current = current->next;
	}
	return (0);
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
	if (!sh->source_line || ft_strchr(sh->source_line, '\\')
		|| ft_strchr(sh->source_line, ';'))
		return (sh->status = E_SYNTAX);
	if (!validate_input(sh))
		return (sh->status = E_SYNTAX);
	if (!tokenize_line(sh))
		return (sh->status = E_SYNTAX);
	if (sh->debug & DEBUG_LEX)
		debug_print_token_list(sh->token_list);
	return (check_token(sh));
}
