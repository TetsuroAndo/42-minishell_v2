/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_err_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:28:57 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 13:43:08 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

static const char	*get_redir_in_token_str(int count)
{
	if (count == 3)
		return ("newline");
	else if (count == 4)
		return ("<");
	else if (count == 5)
		return ("<<");
	else
		return ("<<<");
}

static const char	*get_redir_out_token_str(int count)
{
	if (count == 2)
		return ("newline");
	else if (count == 3)
		return (">");
	else
		return (">>");
}

static const char	*get_pipe_token_str(int count)
{
	if (count == 3)
		return ("|");
	else
		return ("||");
}

static const char	*get_paren_semicolon_token_str(int count, int r, int l)
{
	if (r)
		return (")");
	if (l)
		return ("(");
	if (count == 1 && !r && !l)
		return (";");
	return (";;");
}

void	print_error_message(t_token_type token_type, int count)
{
	const char	*token_str;

	if (token_type == TT_SEMICOLON && count == 1)
		token_str = get_paren_semicolon_token_str(count, 0, 0);
	else if (token_type == TT_REDIR_IN)
		token_str = get_redir_in_token_str(count);
	else if (token_type == TT_REDIR_OUT)
		token_str = get_redir_out_token_str(count);
	else if (token_type == TT_PIPE)
		token_str = get_pipe_token_str(count);
	else if (token_type == TT_SEMICOLON || token_type == TT_LPAREN
		|| token_type == TT_RPAREN)
		token_str = get_paren_semicolon_token_str(count,
				token_type == TT_RPAREN, token_type == TT_LPAREN);
	else
		token_str = "newline";
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)token_str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}
