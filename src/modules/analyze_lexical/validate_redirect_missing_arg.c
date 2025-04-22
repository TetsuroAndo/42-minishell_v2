/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirect_missing_arg.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:22:01 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 14:51:27 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief リダイレクト記号の後に引数がない場合のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @return int 0: エラーなし, 1: エラーあり
 */
int	validate_redirect_missing_arg(const char *line, size_t *pos)
{
	t_token_type	op;
	size_t			temp_pos;

	op = get_two_char_op(&line[*pos], NULL);
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
	if (!line[temp_pos] || get_one_char_op(line[temp_pos]) != TT_ERROR
		|| get_two_char_op(&line[temp_pos], NULL) != TT_ERROR)
		return (ft_putstr_fd(ES_TOKEN, STDERR_FILENO), 1);
	return (0);
}
