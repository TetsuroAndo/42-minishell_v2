/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirect_missing_arg.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:22:01 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 15:22:45 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

int	validate_redirect_missing_arg(const char *line, size_t *pos)
{
	t_token_type	op;
	size_t			temp_pos;

	op = get_two_char_op(&line[*pos]);
	if (op == TT_ERROR)
		op = get_one_char_op(line[*pos]);
	if (op != TT_REDIR_IN && op != TT_REDIR_OUT && op != TT_APPEND
		&& op != TT_HEREDOC)
		return (0);
	if (op == TT_APPEND || op == TT_HEREDOC)
		temp_pos = *pos + 2;
	else
		temp_pos = *pos + 1;
	skip_spaces(line, &temp_pos);
	if (!line[temp_pos] || is_operator(line, temp_pos))
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	return (0);
}
