/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:11:26 by tomsato          ###   ########.fr       */
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
	if (sh->debug & DEBUG_LEX)
		debug_print_token_list(sh->token_list);
	return (E_NONE);
}
