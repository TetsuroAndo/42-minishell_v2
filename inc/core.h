/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:23:28 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 17:33:23 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef CORE_H
# define CORE_H

# include "libft.h"
# include "libms.h"
# include "ms_ast.h"
# include "ms_debug.h"
# include "ms_err.h"
# include "ms_readline.h"
# include "ms_signal.h"
# include "ms_token.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef DEBUG_MODE
#  define DEBUG_MODE DEBUG_ALL
# endif

extern volatile sig_atomic_t	g_signal_status;

typedef struct s_shell
{
	char *bin_name; // 実行バイナリ名 (./minishell)
	// 解析
	char *source_line;  // 入力行
	t_list *token_list; // トークンリスト
	t_list						*token_list_syn;
	t_ast *ast; // 構文木
	// status
	t_status status; // 終了ステータス
	int exit_flag;   // 終了フラグ
	// 環境管理
	t_list *env_map;    // 環境変数マップ
	char *env_spc[128]; // 環境変数特殊文字
	char cwd[PATH_MAX]; // 現在作業ディレクトリ
	int env_updated;    // 環境変数変更フラグ
	// リソース管理
	int stdin_backup;  // 標準入力バックアップ
	int stdout_backup; // 標準出力バックアップ
	int stderr_backup; // 標準エラー出力バックアップ
	// ガベージコレクション
	t_list *gcsh; // shellプロセスごとの自動解放対象のメモリリスト
	t_list *gcli; // Line ごとの自動解放対象のメモリリスト
	// その他
	int interactive; // インタラクティブモード
	int debug;       // デバッグモード
}								t_shell;

/*
** シェルの初期化と終了処理
*/
void							line_init(t_shell *shell);
t_shell							*shell_init(char **env, char *bin_name);
void							shell_cleanup(t_shell *shell);
void							shell_exit(t_shell *shell, int status);

#endif
