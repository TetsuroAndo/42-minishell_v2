/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_varname.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 23:55:27 by teando            #+#    #+#             */
/*   Updated: 2025/05/11 01:01:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief 変数名を検出し、展開結果をバッファに追加する
 *
 * @param buf 		結果を格納するバッファへのポインタ
 * @param in 		変数名を含む入力文字列
 * @param shell 	シェル情報
 * @return size_t	変数名inを何文字進めたか
 */
size_t	extract_varname(char **buf, char *in, t_shell *sh)
{
	size_t	klen;
	char	*key;
	char	*val;

	if (sh->debug & DEBUG_SEM)
		ft_dprintf(STDERR_FILENO, "[EXPAND_VAR]: %s [POINTER]: %p\n", in, in);
	klen = 1;
	if (ft_isalnum(in[0]))
	{
		while (ft_isalnum_under(in[klen]))
			++klen;
	}
	key = ms_substr_gcli(in, 0, klen, sh);
	if (!key || !key[0])
		return (0);
	val = ms_getenv(key, sh);
	if (!val)
		val = ms_strdup_gcli("", sh);
	ft_gc_track(sh->gcli, val);
	*buf = ms_strjoin_gcli(*buf, val, sh);
	return (klen);
}
