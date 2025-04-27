/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_valid_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:57:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:12:04 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/**
 * @brief クォート状態を更新する
 *
 * @param c 現在の文字
 * @param quote 現在のクォート状態
 * @param prev 直前の文字
 * @return char 更新後のクォート状態
 */
static char	update_quote_state(char c, char quote, char prev)
{
	(void)prev;
	if (!quote && (c == '\'' || c == '"' || c == '`'))
		return (c);
	return (quote);
}

/**
 * @brief 文字をコピーするかスキップするか処理する
 *
 * @param s 入力文字列
 * @param i 現在の位置へのポインタ
 * @param out 出力文字列
 * @param j 出力位置へのポインタ
 * @param quote 現在のクォート状態
 * @param prev 直前の文字へのポインタ
 */
static void	copy_or_skip_char(const char *s, size_t *i, char *out, size_t *j,
		char *quote, char *prev)
{
	char	new_quote;

	new_quote = update_quote_state(s[*i], *quote, *prev);
	if (new_quote != *quote)
	{
		*quote = new_quote;
		(*i)++;
	}
	else
		out[(*j)++] = s[(*i)++];
	*prev = s[*i - 1];
}

/**
 * @brief 文字列からクォートを取り除く
 *
 * @param s 入力文字列
 * @param sh シェル情報
 * @return char* クォートを取り除いた新しい文字列
 */
char	*trim_valid_quotes(const char *s, t_shell *sh)
{
	char	*out;
	size_t	i;
	size_t	j;
	char	quote;
	char	prev;

	i = 0;
	j = 0;
	quote = 0;
	prev = '\0';
	if (!s)
		return (ms_strdup("", sh));
	out = xmalloc(ft_strlen(s) + 1, sh);
	while (s[i])
		copy_or_skip_char(s, &i, out, &j, &quote, &prev);
	out[j] = '\0';
	if (quote)
		return (xfree((void **)&out), ms_strdup(s, sh));
	return (out);
}
