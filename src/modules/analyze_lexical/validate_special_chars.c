/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_special_chars.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:38:31 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 17:59:59 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_lex.h"

/**
 * @brief 連続したリダイレクト記号の数をカウントする
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param symbol カウントする記号
 * @return int 連続した記号の数
 */
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

/**
 * @brief リダイレクト後の記号のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @return int 0: エラーなし, 1: エラーあり
 */
static int	check_redirect_symbol_error(const char *line, size_t *pos)
{
	size_t	temp_pos;

	temp_pos = *pos + 1;
	skip_spaces(line, &temp_pos);
	if (line[temp_pos] == '|' || line[temp_pos] == '&' || line[temp_pos] == ';'
		|| line[temp_pos] == '(' || line[temp_pos] == ')')
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	return (0);
}

/**
 * @brief 過剰なリダイレクト記号のエラーチェック
 *
 * @param symbol リダイレクト記号
 * @param count 連続する記号の数
 * @param pos 現在の位置へのポインタ
 * @return int 0: エラーなし, 1: エラーあり
 */
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

/**
 * @brief 記号の後に特殊文字が続く場合のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @return int 0: エラーなし, 1: エラーあり
 */
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
