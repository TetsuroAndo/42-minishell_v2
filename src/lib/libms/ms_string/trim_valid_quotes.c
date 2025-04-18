/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_valid_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:57:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 23:58:57 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

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
