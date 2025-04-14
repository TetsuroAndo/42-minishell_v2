/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:07 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 20:27:19 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
 * 成功: 1, 失敗: 0
 */
int	add_token(t_shell *shell, t_lexical_token *new_token)
{
	t_list	*new_node;

	if (!new_token)
		return (0);
	new_node = xlstnew((void *)new_token, shell);
	if (!new_node)
	{
		free(new_token->value);
		free(new_token);
		return (0);
	}
	if (!shell->token_list)
		shell->token_list = new_node;
	else
		ft_lstadd_back(&shell->token_list, new_node);
	return (1);
}
