/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:27:41 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 16:58:35 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief 次のトークンの開始位置と長さを取得する
 *
 * @param p 探索開始位置
 * @param start トークン開始位置へのポインタ（結果格納用）
 * @return size_t トークンの長さ、トークンがなければ0
 */
static size_t	get_next_token(char **p, char **start)
{
	char	q;
	char	*pos;

	pos = *p;
	while (*pos && isspace((unsigned char)*pos))
		pos++;
	if (!*pos)
		return (0);
	*start = pos;
	if (*pos == '"' || *pos == '\'')
	{
		q = *pos++;
		while (*pos && *pos != q)
			pos++;
		if (*pos == q)
			pos++;
	}
	else
	{
		while (*pos && !isspace((unsigned char)*pos) && *pos != '"'
			&& *pos != '\'')
			pos++;
	}
	*p = pos;
	return (pos - *start);
}

/**
 * @brief 文字列をクォートを考慮して分割する
 *
 * - 空白（スペース・タブ・改行など）で区切る
 * - シングルクォート(')またはダブルクォート(")で囲まれた部分は、
 *   中の空白を含めて１トークンとして扱う
 *
 * @param str 分割する文字列
 * @param sh シェル情報
 * @return char** 分割された文字列の配列（NULLで終端）
 */
char	**split_with_quote(char *str, t_shell *sh)
{
	char	**result;
	size_t	count;
	size_t	i;
	char	*p;
	char	*token_start;
	size_t	token_len;

	if (!str)
		return (NULL);
	count = count_aft_wc_tok(str);
	result = xmalloc_gcline(sizeof(char *) * (count + 1), sh);
	p = str;
	i = 0;
	while (i < count)
	{
		token_len = get_next_token(&p, &token_start);
		if (token_len == 0)
			break ;
		result[i] = ms_substr_gcli(token_start, 0, token_len, sh);
		i++;
	}
	result[i] = NULL;
	return (result);
}
