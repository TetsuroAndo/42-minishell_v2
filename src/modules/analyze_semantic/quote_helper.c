/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:57:56 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 17:08:19 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/**
 * @brief クォートを取り除いた文字列を作成する
 * 
 * @param s 入力文字列
 * @param sh シェル情報
 * @return char* クォートを取り除いた新しい文字列
 */
char	*strip_quotes(const char *s, t_shell *sh)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*out;

	i = 0;
	j = 0;
	len = ft_strlen(s);
	out = xmalloc(len + 1, sh);
	while (i < len)
	{
		if (s[i] == '\'' || s[i] == '"')
			i++;
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
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
