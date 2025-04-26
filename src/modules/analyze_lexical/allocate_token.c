/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:07 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 23:19:58 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/*
 * トークン構造体を動的に確保し、type と value を設定して返す。
 */
t_lexical_token	*create_token(t_token_type type, char *value, t_shell *shell)
{
	t_lexical_token	*token;

	token = (t_lexical_token *)xmalloc(sizeof(t_lexical_token), shell);
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	return (token);
}

/*
 * 新規トークンをリストに追加する。
 * ヒアドキュメントトークンの場合はhandle_heredocを呼び出して処理する。
 * 成功: 1, 失敗: 0
 */
int	add_token(t_shell *shell, t_lexical_token *new_token)
{
	t_list	*new_node;

	if (!new_token)
		return (1);
	if (new_token->type == TT_HEREDOC)
		new_node = handle_heredoc(new_token, shell);
	else
		new_node = xlstnew(new_token, shell);
	if (!new_node)
		return (1);
	if (!shell->token_list)
		shell->token_list = new_node;
	else
		ft_lstadd_back(&shell->token_list, new_node);
	return (0);
}
