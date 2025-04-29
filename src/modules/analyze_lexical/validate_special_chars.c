/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_special_chars.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:38:31 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:12:53 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

static int	cnt_symbols(const char *line, size_t pos, char symbol)
{
	int	count;

	count = 0;
	while (line[pos] && line[pos] == symbol)
	{
		count++;
		pos++;
	}
	return (count);
}

static int	check_redirect_symbol_error(const char *line, size_t *pos)
{
	size_t	temp_pos;

	temp_pos = *pos + 1;
	skip_spaces(line, &temp_pos);
	if (line[temp_pos] == '|' || line[temp_pos] == '&' || line[temp_pos] == '('
		|| line[temp_pos] == ')')
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	return (0);
}

static int	check_excessive_redirect(char symbol, int count, size_t *pos)
{
	if ((symbol == '<' && count > 2) || (symbol == '>' && count > 2)
		|| (symbol == '|' && count > 2))
	{
		*pos += count;
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	}
	return (0);
}

int	validate_special_chars(const char *line, size_t *pos)
{
	t_token_type	op;
	int				count;
	char			symbol;

	op = get_one_char_op(line[*pos]);
	if (op != TT_REDIR_IN && op != TT_REDIR_OUT && op != TT_PIPE)
		return (0);
	symbol = line[*pos];
	count = cnt_symbols(line, *pos, symbol);
	if ((symbol == '>' && count == 1 && line[*pos + 1] == '|') || (symbol == '>'
			&& count == 2 && line[*pos + 2] == '|') || (symbol == '>'
			&& count == 1 && line[*pos + 1] == '&'))
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	if ((symbol == '>' || symbol == '<') && count == 1
		&& check_redirect_symbol_error(line, pos))
		return (1);
	return (check_excessive_redirect(symbol, count, pos));
}
