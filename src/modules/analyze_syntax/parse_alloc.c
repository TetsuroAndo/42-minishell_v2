/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_alloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:10:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:15:03 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

/**
 * Duplicate the current token and advance the token list
 */
t_lexical_token	*tok_pop_dup(t_pl *pl, t_shell *sh)
{
	t_lexical_token	*src;
	t_lexical_token	*cp;

	src = tok_peek(pl);
	if (!src)
		return (NULL);
	cp = xmalloc(sizeof(*cp), sh);
	ft_memcpy(cp, src, sizeof(*cp));
	if (src->value)
		cp->value = ms_strdup(src->value, sh);
	ms_listshift(&pl->lst);
	return (cp);
}

/**
 * Create a new args structure
 */
t_args	*args_new(t_shell *sh)
{
	t_args	*a;

	a = xmalloc(sizeof(*a), sh);
	a->fds[0] = -1;
	a->fds[1] = -1;
	a->pid = -1;
	return (a);
}

/**
 * Create a new AST node
 */
t_ast	*ast_make(t_ntype type, t_ast *l, t_ast *r, t_shell *sh)
{
	t_ast	*n;

	n = xmalloc(sizeof(*n), sh);
	n->ntype = type;
	n->left = l;
	n->right = r;
	return (n);
}
