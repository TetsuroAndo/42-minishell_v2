/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 01:57:58 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

t_token_type	get_two_char_op(const char *s)
{
	if (!s || !s[0] || !s[1])
		return (TT_ERROR);
	if (ft_strncmp(s, ">>", 2) == 0)
	{
		return (TT_APPEND);
	}
	if (ft_strncmp(s, "<<", 2) == 0)
	{
		return (TT_HEREDOC);
	}
	if (ft_strncmp(s, "&&", 2) == 0)
	{
		return (TT_AND_AND);
	}
	if (ft_strncmp(s, "||", 2) == 0)
	{
		return (TT_OR_OR);
	}
	return (TT_ERROR);
}

t_token_type	get_one_char_op(int c)
{
	static const t_token_type	op_map[128] = {['|'] = TT_PIPE,
	['>'] = TT_REDIR_OUT, ['<'] = TT_REDIR_IN, ['('] = TT_LPAREN,
	[')'] = TT_RPAREN};

	if (c >= 0 && c <= 127 && op_map[(int)c] != 0)
		return (op_map[(int)c]);
	return (TT_ERROR);
}

int	is_operator(const char *line, size_t pos)
{
	if (get_two_char_op(&line[pos]) != TT_ERROR)
		return (1);
	if (get_one_char_op(line[pos]) != TT_ERROR)
		return (1);
	return (0);
}

/**
 * @brief 単語（トークン）を読み取る
 *
 * line[pos]から始まる単語を読み取り、その内容を返す。
 * 単語の終わりは空白文字、演算子、または文字列の終わりで判断される。
 * 引用符で始まる場合や途中に引用符がある場合も、一つの単語として処理される。
 *
 * @param line 処理対象の文字列
 * @param pos 現在の位置を示すポインタ（処理後は単語の次の位置に更新される）
 * @param shell ステータスを保持する構造体
 * @return 読み取った単語。エラー時はNULL
 */
char	*read_word(const char *line, size_t *pos, t_shell *shell)
{
	size_t	start;

	start = *pos;
	while (line[*pos])
	{
		if (line[*pos] == '\'' || line[*pos] == '"')
		{
			if (skip_quoted_word(line, pos, shell))
				return (NULL);
			continue ;
		}
		if (line[*pos] == '$' && line[*pos + 1] == '(')
		{
			skip_dollar_paren(line, &(*pos));
			continue ;
		}
		if (ft_isspace(line[*pos]) || is_operator(line, *pos))
			break ;
		(*pos)++;
	}
	return (ms_substr(line, start, (*pos - start), shell));
}
