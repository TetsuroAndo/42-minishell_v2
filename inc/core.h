#ifndef CORE_H
# define CORE_H

# include "libft.h"
# include "libms.h"
# include "ms_ast.h"
# include "ms_debug.h"
# include "ms_readline.h"
# include "ms_signal.h"
# include "ms_token.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define DEFAULT_DEBUG DEBUG_ALL

extern volatile sig_atomic_t	g_signal_status;

typedef enum e_status
{
	E_NONE = 0,
	E_SYSTEM = 3,
	E_ALLOCATE = 1,
	E_ENV_KEY = 1,
	E_SYNTAX = 1,
	E_PIPE = 1,
	E_FILE,
	E_ARGUMENT,
	E_NOT_BUITIN_CMD = -2,
	E_IS_DIR = 126,
	E_IS_FILE = 126,
	E_NOT_DIR = 126,
	E_NOT_FILE = 126,
	E_PERMISSION_DENIED = 126,
	E_NOT_EXECUTABLE = 126,
	E_NOT_FOUND = 127,
	E_COMMAND_NOT_FOUND = 127,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
}								t_status;

typedef struct s_shell
{
	char *bin_name;		// 実行バイナリ名 (./minishell)
	// 解析
	char *source_line;  // 入力行
	t_list *token_list; // トークンリスト
	t_list *token_list_syn;
	t_ast *ast;         // 構文木
	// status
	t_status status; // 終了ステータス
	int exit_flag;   // 終了フラグ
	// 環境管理
	t_list *env_map;    // 環境変数マップ
	char *env_spc[128]; // 環境変数特殊文字
	char cwd[PATH_MAX]; // 現在作業ディレクトリ
	// リソース管理
	int stdin_backup;  // 標準入力バックアップ
	int stdout_backup; // 標準出力バックアップ
	int stderr_backup; // 標準エラー出力バックアップ
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
