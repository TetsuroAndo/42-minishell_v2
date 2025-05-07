/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/05/07 17:12:38 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

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

t_status	mod_lex(t_shell *sh)
{
	sh->token_list = NULL;
	sh->status = E_NONE;
	if (!sh->source_line)
		return (sh->status = E_SYNTAX);
	if (!validate_input(sh))
		return (sh->status = E_SYNTAX);
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
