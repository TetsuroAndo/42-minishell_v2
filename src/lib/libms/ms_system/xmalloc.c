/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:42:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 17:30:46 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"

/**
 * @brief 失敗時に終了する安全なメモリ確保ラッパー関数
 *
 * この関数は以下の処理を行います：
 * 1. 指定されたサイズのメモリ確保を試みる
 * 2. 確保したメモリをft_bzeroで0に初期化
 * 3. 確保に失敗した場合はプログラムを終了
 *
 * @param size 確保するメモリのバイト数
 * @param shell エラー処理用のシェルメイン情報構造体
 * @return void* 確保したメモリへのポインタ
 */
void	*xmalloc(size_t size, t_shell *shell)
{
	char	*memory;

	memory = malloc(size);
	if (!memory)
	{
		shell_exit(shell, E_SYSTEM);
	}
	ft_bzero(memory, size);
	return ((void *)memory);
}
