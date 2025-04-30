/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:45:42 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/30 12:13:22 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

t_ast	*parse_list(t_pl *pl, t_shell *sh)
{
	return (parse_and_or(pl, sh));
}

t_status	mod_syn(t_shell *sh)
{
	t_pl	pl;
	t_ast	*ast;

	pl.lst = sh->token_list;
	ast = parse_list(&pl, sh);
	if (!ast && !tok_peek(&pl) && !tok_is_eof(tok_peek(&pl)->type))
	{
		if (ast)
			free_ast(&ast);
		return (synerr(sh, "syntax error near unexpected token"));
	}
	sh->ast = ast;
	if (sh->debug & DEBUG_SYN)
		debug_print_ast(ast, sh);
	return (E_NONE);
}
