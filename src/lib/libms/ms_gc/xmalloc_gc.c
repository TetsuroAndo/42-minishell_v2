/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc_gc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:53:41 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 23:12:27 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

/**
 * @brief ガベージコレクション対応のメモリ確保関数
 *
 * この関数は以下の処理を行います：
 * 1. xmallocで指定されたサイズのメモリを確保
 * 2. 確保したメモリをシェルのGCリストに追加
 * 3. シェル終了時に自動的に解放される
 *
 * @param size 確保するメモリのバイト数
 * @param shell シェルメイン情報構造体
 * @return void* 確保したメモリへのポインタ
 */
void	*xmalloc_gcshell(size_t n, t_shell *sh)
{
	void	*p;

	p = xmalloc(n, sh);
	ft_lstadd_back(&sh->gcsh, xlstnew(p, sh));
	return (p);
}

void	*xmalloc_gcline(size_t n, t_shell *sh)
{
	void	*p;

	p = xmalloc(n, sh);
	ft_lstadd_back(&sh->gcli, xlstnew(p, sh));
	return (p);
}
