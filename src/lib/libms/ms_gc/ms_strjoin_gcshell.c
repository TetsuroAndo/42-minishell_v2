/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strjoin_gcshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:46:13 by teando            #+#    #+#             */
/*   Updated: 2025/04/23 16:19:22 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/**
 * @brief システムコール失敗時にshell_exitを呼び出すラッパー関数
 * 
 * @param s1 結合する最初の文字列
 * @param s2 結合する2番目の文字列
 * @param sh シェル構造体
 * @return char* 結合された新しい文字列
 */
char	*ms_strjoin_gcshell(char const *s1, char const *s2, t_shell *sh)
{
	char	*r;
	size_t	size;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	r = (char *)xmalloc_gcshell(sizeof(char) * size, sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	ft_strlcpy(r, s1, size);
	ft_strlcat(r, s2, size);
	return (r);
}

/**
 * @brief システムコール失敗時にshell_exitを呼び出すラッパー関数
 * s1とs2を結合し、両方の文字列を解放する
 * 
 * @param s1 結合する最初の文字列（解放される）
 * @param s2 結合する2番目の文字列（解放される）
 * @param sh シェル構造体
 * @return char* 結合された新しい文字列
 */
char	*ms_strjoin_gcshell_f2(char const *s1, char const *s2, t_shell *sh)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = ms_strjoin_gcshell(s1, s2, sh);
	if (!str)
		shell_exit(sh, E_SYSTEM);
	if (s1)
		xfree((void **)&s1);
	if (s2)
		xfree((void **)&s2);
	return (str);
}
