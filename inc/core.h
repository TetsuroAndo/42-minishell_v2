#ifndef CORE_H
#define CORE_H

#include "ms_token.h"
#include "ms_module.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

/*
** シグナルステータス
*/
extern volatile sig_atomic_t g_signal_status;

/*
** シェルステータス
*/
typedef enum e_status
{
    E_NONE = 0,
    E_ALLOCATE = 1,
    E_ENV_KEY = 1,
    E_SYNTAX = 1,
    E_PIPE = 1,
    E_FILE,
    E_ARGUMENT,
    E_NOT_BUITIN_CMD = -2,
    E_COMMAND_NOT_FOUND = 127,
    E_SIGINT = 130,
    E_SIGQUIT = 131,
} t_status;

/*
** シェル構造体
*/
typedef struct s_shell
{
    // コア状態
    char *source_line;  // 入力行
    t_list *token_list; // トークンリスト
    t_ast *ast;         // 構文木
    t_status status;    // 終了ステータス
    int exit_flag;      // 終了フラグ

    // モジュール管理
    t_module_registry *modules; // モジュールレジストリ
    int module_flags;           // モジュールフラグ

    // 環境管理（ENVモジュールに移行予定）
    t_list *env_map;    // 環境変数マップ
    char *env_spc[128]; // 環境変数特殊文字
    char cwd[PATH_MAX]; // 現在作業ディレクトリ

    // リソース管理
    int stdin_backup;  // 標準入力バックアップ
    int stdout_backup; // 標準出力バックアップ
    int stderr_backup; // 標準エラー出力バックアップ

    // その他
    int interactive; // インタラクティブモード
} t_shell;

/*
** シェルの初期化と終了処理
*/
int shell_init(t_shell *shell, char **env, int module_flags);
void shell_cleanup(t_shell *shell);
void shell_exit(t_shell *shell, int status);

/*
** シェルループ
*/
void shell_loop(t_shell *shell);

#endif