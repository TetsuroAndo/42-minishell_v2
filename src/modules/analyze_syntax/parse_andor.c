/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_andor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:35:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:15:00 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

/**
 * Parse a binary expression using a table-driven approach
 */
static t_ast	*parse_binexpr(t_pl *pl, t_shell *sh, t_ast *(*sub)(t_pl *,
			t_shell *), const t_binop tbl[])
{
	t_ast			*left;
	t_ast			*right;
	t_lexical_token	*tok;
	int				i;

	left = sub(pl, sh);
	if (!left)
		return (NULL);
	tok = tok_peek(pl);
	while (tok)
	{
		i = 0;
		while (tbl[i].tt && tbl[i].tt != tok->type)
			i++;
		if (tbl[i].tt == 0)
			break ;
		ms_listshift(&pl->lst);
		right = sub(pl, sh);
		if (!right)
			return (free_ast(&left), NULL);
		left = ast_make(tbl[i].ntype, left, right, sh);
		tok = tok_peek(pl);
	}
	return (left);
}

/**
 * Parse an AND/OR expression (pipeline && pipeline || pipeline ...)
 * ========== And/Or ==========
 *  and_or ::= pipeline ( ( '&&' | '||' ) pipeline )*
 */
t_ast	*parse_and_or(t_pl *pl, t_shell *sh)
{
	static const t_binop	tbl[] = {{TT_AND_AND, NT_AND}, {TT_OR_OR, NT_OR},
	{0, 0}};

	return (parse_binexpr(pl, sh, parse_pipeline, tbl));
}
