/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_substr_gcshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:46:13 by teando            #+#    #+#             */
/*   Updated: 2025/04/23 15:46:13 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/**
 * @brief システムコール失敗時にshell_exitを呼び出すラッパー関数
 * 文字列の指定された位置から指定された長さの部分文字列を取得する
 * 
 * @param s 元の文字列
 * @param start 開始位置
 * @param len 取得する長さ
 * @param sh シェル構造体
 * @return char* 部分文字列
 */
char	*ms_substr_gcshell(char const *s, unsigned int start, size_t len, t_shell *sh)
{
	char	*r;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ms_strdup_gcshell("", sh));
	r = ms_strndup_gcshell(s + ft_strnlen(s, start), len, sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	return (r);
}

/**
 * @brief システムコール失敗時にshell_exitを呼び出すラッパー関数
 * 指定された区切り文字以降の部分文字列を取得する
 * 
 * @param s 元の文字列
 * @param delimiter 区切り文字
 * @param sh シェル構造体
 * @return char* 区切り文字以降の部分文字列
 */
char	*ms_substr_r_gcshell(char const *s, char delimiter, t_shell *sh)
{
	char	*pos;
	char	*r;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcshell("", sh));
	r = ms_strdup_gcshell(pos + 1, sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	return (r);
}

/**
 * @brief システムコール失敗時にshell_exitを呼び出すラッパー関数
 * 指定された区切り文字までの部分文字列を取得する
 * 
 * @param s 元の文字列
 * @param delimiter 区切り文字
 * @param sh シェル構造体
 * @return char* 区切り文字までの部分文字列
 */
char	*ms_substr_l_gcshell(const char *s, char delimiter, t_shell *sh)
{
	char	*pos;
	size_t	len;
	char	*r;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcshell(s, sh));
	len = (size_t)(pos - s);
	r = ms_strndup_gcshell(s, len, sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	return (r);
}
