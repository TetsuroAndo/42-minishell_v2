/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:22:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:15:06 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

/**
 * Check if token is a redirection token
 */
int	tok_is_redir(t_token_type t)
{
	return ((t & 0xFF00) == TM_REDIR);
}

/**
 * Check if token is an EOF token
 */
int	tok_is_eof(t_token_type t)
{
	return (t == TT_EOF);
}

/**
 * Peek at the current token without consuming it
 */
t_lexical_token	*tok_peek(t_pl *pl)
{
	if (!pl || !pl->lst || !pl->lst->data)
		return (NULL);
	return ((t_lexical_token *)pl->lst->data);
}

/**
 * Common error reporting function
 */
t_status	synerr(t_shell *sh, const char *msg)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s\n", msg);
	sh->status = E_SYNTAX;
	return (E_SYNTAX);
}
