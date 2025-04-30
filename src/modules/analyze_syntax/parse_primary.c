/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:25:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/30 12:49:33 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

/**
 * Append a redirection token to the AST node
 * Returns false if a redirection was processed (to continue collecting)
 * Returns true if no redirection was found (to stop collecting)
 */
static int	append_redir(t_pl *pl, t_ast *n, t_shell *sh)
{
	t_lexical_token	*tok;
	t_list			*dup;

	tok = tok_peek(pl);
	if (!tok || !tok_is_redir(tok->type))
		return (0);
	if (*tok->value == '\0' && tok->type != TT_HEREDOC)
	{
		synerr(sh, "syntax error near unexpected token");
		return (0);
	}
	dup = xlstnew(tok_pop_dup(pl, sh), sh);
	if (!n->args->redr)
		n->args->redr = dup;
	else
		ft_lstadd_back(&n->args->redr, dup);
	return (1);
}

/**
 * Parse a command (redirections and words)
 */
static t_ast	*parse_cmd(t_pl *pl, t_shell *sh)
{
	t_ast			*n;
	t_lexical_token	*tok;

	n = ast_make(NT_CMD, NULL, NULL, sh);
	n->args = args_new(sh);
	while (append_redir(pl, n, sh))
		;
	tok = tok_peek(pl);
	while (tok && tok->type == TT_WORD)
	{
		ft_lstadd_back(&n->args->argv, xlstnew(tok_pop_dup(pl, sh), sh));
		tok = tok_peek(pl);
	}
	while (append_redir(pl, n, sh))
		;
	return (n);
}

/**
 * Parse a primary expression (command or subshell)
 * ========== Primary (cmd or '(' list ')') ==========
 *  primary ::= cmd | '(' list ')'
 */
t_ast	*parse_primary(t_pl *pl, t_shell *sh)
{
	t_lexical_token	*tok;
	t_ast			*sub;

	tok = tok_peek(pl);
	if (!tok)
		return (NULL);
	if (tok_is_redir(tok->type) || tok->type == TT_WORD)
		return (parse_cmd(pl, sh));
	if (tok->type == TT_LPAREN)
	{
		ms_listshift(&pl->lst);
		sub = parse_list(pl, sh);
		if (!sub || !tok_peek(pl) || tok_peek(pl)->type != TT_RPAREN)
		{
			synerr(sh, "syntax error: missing ')'");
			free_ast(&sub);
			return (NULL);
		}
		ms_listshift(&pl->lst);
		return (ast_make(NT_SUBSHELL, sub, NULL, sh));
	}
	return (NULL);
}
