#include "core.h"

extern volatile sig_atomic_t g_signal_status;

/**
 * @brief 入力文字列内のクォートが正しく閉じられているかチェックする
 *
 * この関数は以下の処理を行います：
 * - シングルクォートとダブルクォートの状態を個別に追跡
 * - ネストされたクォート（例：'text "more text" text'）を処理
 * - 対となるクォート内のクォートは文字列として扱う
 *
 * @param line チェックする入力文字列
 * @return int クォートが正しく閉じられている場合は1、そうでない場合は0
 */
static int is_quotes_balanced(const char *line)
{
    int single_open;
    int double_open;
    size_t i;

    single_open = 0;
    double_open = 0;
    i = 0;
    while (line[i])
    {
        if (line[i] == '\'' && !double_open)
            single_open = !single_open;
        else if (line[i] == '"' && !single_open)
            double_open = !double_open;
        i++;
    }
    if (!single_open && !double_open)
        return (1);
    return (0);
}

/**
 * @brief readlineを使用して1行の入力を読み取り、履歴に追加する
 *
 * この関数は以下の処理を行います：
 * 1. 指定されたプロンプトで入力を受け付ける
 * 2. 入力が空でない場合、コマンド履歴に追加
 * 3. 入力された行を返す（呼び出し側で解放が必要）
 *
 * @param prompt 表示するプロンプト文字列
 * @return char* 入力された行、EOFまたはエラー時はNULL
 */
static char *read_command_line(const char *prompt)
{
    char *line;

    line = readline(prompt);
    if (line && *line)
    {
        add_history(line);
    }
    return (line);
}

/**
 * @brief クォートが正しく閉じられるまで入力を読み続ける
 *
 * この関数は以下の処理を行います：
 * 1. 最初の行を読み取る
 * 2. クォートが閉じられていない場合、続きの入力を受け付ける
 * 3. 必要に応じて複数行を改行文字で結合
 * 4. クォートが閉じられるかEOF/エラーが発生するまで続ける
 *
 * @param prompt メインのプロンプト文字列
 * @return char* 完全な入力文字列、EOFまたはエラー時はNULL
 */
char *launch_readline(const char *prompt)
{
    char *line;
    char *tmp;
    char *cont_line;

    line = read_command_line(prompt);
    if (!line)
        return (NULL);
    while (!is_quotes_balanced(line))
    {
        if (g_signal_status == SIGINT)
            return (free(line), NULL);
        cont_line = read_command_line("> ");
        if (!cont_line)
            return (free(line), NULL);
        tmp = ft_strjoin(line, "\n");
        free(line);
        line = ft_strjoin_free(tmp, cont_line);
        if (!line)
            return (NULL);
    }
    return (line);
}
