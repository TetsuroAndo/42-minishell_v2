/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_putlex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:56:54 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 18:08:24 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

static const char	*type_to_str(t_token_type t)
{
	static const t_token_type	types[] = {TT_WORD, TT_PIPE, TT_REDIR_IN,
		TT_APPEND, TT_REDIR_OUT, TT_HEREDOC, TT_LPAREN, TT_RPAREN,
		TT_AND_AND, TT_OR_OR, TT_SEMICOLON, TT_EOF, TT_ERROR};
	static const char			*type_strs[] = {"TT_WORD", "TT_PIPE",
		"TT_REDIR_IN", "TT_APPEND", "TT_REDIR_OUT", "TT_HEREDOC",
		"TT_LPAREN", "TT_RPAREN", "TT_AND_AND", "TT_OR_OR",
		"TT_SEMICOLON", "TT_EOF", "TT_ERROR"};
	size_t						i;

	i = 0;
	while (i < sizeof(types) / sizeof(types[0]))
	{
		if (t == types[i])
			return (type_strs[i]);
		i++;
	}
	return ("TT_UNKNOWN");
}

void	debug_print_token_list(t_list *list)
{
	t_lexical_token	*tok;

	printf("Token list:\n");
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
