/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:57:56 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 21:00:32 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_sem.h"

t_quote_state	is_quote_type(int c)
{
	if (c == '"')
		return (QS_DOUBLE);
	if (c == '\'')
		return (QS_SINGLE);
	if (c == '`')
		return (QS_BACK);
	return (QS_NONE);
}

int	check_qs(int c, t_sem *sem)
{
	if (sem->quote_state == QS_NONE)
		sem->quote_state = is_quote_type(c);
	else if (sem->quote_state == is_quote_type(c))
		sem->quote_state = QS_NONE;
	return (c);
}

static int	is_open_quote(char c, char quote)
{
	return (!quote && (c == '\'' || c == '"'));
}

static int	is_close_quote(char c, char quote, char prev)
{
	return (quote && c == quote && prev != '\\');
}

/**
 * @brief クォートを取り除いた文字列を作成する
 *
 * @param s 入力文字列
 * @param sh シェル情報
 * @return char* クォートを取り除いた新しい文字列
 */
char	*trim_valid_quotes(const char *s, t_shell *sh)
{
	char	out[PATH_MAX];
	char	quote;
	char	prev;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	quote = 0;
	while (s && s[i])
	{
		if (is_open_quote(s[i], quote))
			quote = s[i], i++;
		else if (is_close_quote(s[i], quote, prev))
			quote = 0, i++;
		else
			out[j++] = s[i++];
		prev = s[i - 1];
	}
	out[j] = '\0';
	if (quote)
		return (ms_strdup(s, sh));
	return (ms_strdup(out, sh));
}

/**
 * @brief 文字列にクォートが含まれているかチェックする
 *
 * @param s チェックする文字列
 * @return int クォートが含まれていれば1、なければ0
 */
int	is_quoted(const char *s)
{
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		else
			++s;
	}
	return (0);
}
