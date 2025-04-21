/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_launcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 01:47:27 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 07:03:12 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

/*──────────────────────────
 * 1) 単純連結
 *──────────────────────────*/
static int	__simple_link(char path[PATH_MAX + 1], const char *src, int mode,
		t_shell *sh)
{
	ms_path_cleancpy(path, sh->cwd, PATH_MAX + 1);
	if (path[0] && path[ft_strlen(path) - 1] != '/')
		ms_path_cleancat(path, "/", PATH_MAX + 1);
	ms_path_cleancat(path, src, PATH_MAX + 1);
	return (access(path, mode));
}

/**
 * @brief パスの種類を判別し、適切な解決方法を選択する
 *
 * この関数は以下の処理を行います：
 * 1. パスの先頭文字を確認し、以下の種類に分類：
 *    - 絶対パス（'/'で始まる）
 *    - ルートディレクトリ（'/'のみ）
 *    - ホームディレクトリ（'~'で始まる）
 *    - 相対パス 単純連結（'.'がない）
 *    - 相対パス（'.'で始まる）
 * 2. 各種類に応じた解決関数を呼び出す
 *
 * @param path 解決されたパスを格納する配列
 * @param src 元のパス文字列
 * @param mode アクセス権限フラグ
 * @param sh シェル情報構造体
 * @return int 0:成功、-1:失敗
 */
int	path_launcher(char path[], const char *src, int mode, t_shell *sh)
{
	if (src[0] == '/' && access(src, mode) == 0)
	{
		ms_path_cleancpy(path, src, PATH_MAX + 1);
		return (0);
	}
	if (src[0] == '/' && src[1] == '\0')
	{
		ms_path_cleancpy(path, "/", 2);
		return (0);
	}
	if (src[0] == '~')
		return (path_home(path, src, mode, sh));
	if (!ft_strstr(src, "."))
		return (__simple_link(path, src, mode, sh));
	return (path_relative(path, src, mode, sh));
}
