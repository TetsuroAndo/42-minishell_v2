/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_put_lex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:56:54 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 22:57:49 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief トークンタイプを文字列表現に変換する
 *
 * @param t 変換するトークンタイプ
 * @return トークンタイプの文字列表現
 */
static const char *type_to_str(t_token_type t)
{
	if (t == TT_WORD)
		return ("TT_WORD");
	else if (t == TT_PIPE)
		return ("TT_PIPE");
	else if (t == TT_REDIR_IN)
		return ("TT_REDIR_IN");
	else if (t == TT_APPEND)
		return ("TT_APPEND");
	else if (t == TT_REDIR_OUT)
		return ("TT_REDIR_OUT");
	else if (t == TT_HEREDOC)
		return ("TT_HEREDOC");
	else if (t == TT_LPAREN)
		return ("TT_LPAREN");
	else if (t == TT_RPAREN)
		return ("TT_RPAREN");
	else if (t == TT_AND_AND)
		return ("TT_AND_AND");
	else if (t == TT_OR_OR)
		return ("TT_OR_OR");
	else if (t == TT_SEMICOLON)
		return ("TT_SEMICOLON");
	else if (t == TT_EOF)
		return ("TT_EOF");
	else if (t == TT_ERROR)
		return ("TT_ERROR");
	return ("TT_UNKNOWN");
}

/**
 * @brief デバッグ用にトークンリストの内容を表示する
 *
 * @param list 表示するトークンリスト
 */
void debug_print_token_list(t_list *list)
{
	t_lexical_token *tok;

	while (list)
	{
		tok = (t_lexical_token *)list->data;
		printf("type: %s, value: ", type_to_str(tok->type));
		if (tok->value)
			printf("[%s]\n", tok->value);
		else
			printf("NULL\n");
		list = list->next;
	}
}
