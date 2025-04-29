/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:30:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:15:04 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

t_ast	*parse_pipeline(t_pl *pl, t_shell *sh)
{
	t_ast			*left;
	t_ast			*right;
	t_lexical_token	*tok;

	left = parse_primary(pl, sh);
	if (!left)
		return (NULL);
	tok = tok_peek(pl);
	while (tok && tok->type == TT_PIPE)
	{
		ms_listshift(&pl->lst);
		right = parse_primary(pl, sh);
		if (!right)
		{
			free_ast(&left);
			return (NULL);
		}
		left = ast_make(NT_PIPE, left, right, sh);
		tok = tok_peek(pl);
	}
	return (left);
}
