/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:39 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 12:58:27 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_sem.h"

// wordを引数にコネコネする関数→ft_lstiterみたいなやつで全部に適用したら良さそうかも
// 文字リテラル
// → リダイレクト
// → CMD 解決
//   → ビルドイン
//   → PATH 解決
//     → minishell: %s: command not found
// → 展開可能文字 * $

/* -------------------------------------------------------------------------- */
/* Environment                                                                */
/* -------------------------------------------------------------------------- */

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

	klen = 1;
	while (ft_isalnum_under(in[klen]))
		++klen;
	key = ms_substr(in, 0, klen, sh);
	val = ms_getenv(key, sh);
	if (!val)
		val = ms_strdup("", sh);
	*buf = xstrjoin_free2(*buf, val, sh);
	free(key);
	return (klen);
}

/**
 * @brief バックスラッシュと$記号を処理し、環境変数を展開する
 *
 * @param in 処理する入力文字列
 * @param shell シェル情報
 * @return char* 展開後の文字列
 */
char	*handle_env(char *in, t_shell *sh)
{
	t_sem	s;
	size_t	i;

	s.buf = ms_strdup("", sh);
	s.quote_state = QS_NONE;
	while (*in)
	{
		i = 0;
		while (check_qs(in[i], &s) && ((!ft_isbackslash(in[i]) && in[i] != '$')
				|| s.quote_state == QS_SINGLE))
			++i;
		s.buf = xstrjoin_free2(s.buf, ms_substr(in, 0, i, sh), sh);
		in += i;
		if (*in == '$' && s.quote_state != QS_SINGLE)
			in += extract_varname(&s.buf, in + 1, sh) + 1;
		else if (ft_isbackslash(*in) && in[1] != '*'
			&& s.quote_state != QS_SINGLE)
		{
			s.buf = xstrjoin_free2(s.buf, ms_substr(in + 1, 0, 1, sh), sh);
			in += 2;
		}
		else if (*in)
			check_qs(*in++, &s);
	}
	return (s.buf);
}

/* -------------------------------------------------------------------------- */
/* PATH resolve                                                               */
/* -------------------------------------------------------------------------- */

static int	is_builtin(char *cmd)
{
	static const char	*tbl[] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};
	size_t				i;

	i = 0;
	while (tbl[i])
	{
		if (!ft_strcmp(cmd, tbl[i++]))
			return (1);
	}
	return (0);
}

int	resolve_path(char *in, t_shell *sh)
{
	char	**paths;
	char	*test;
	size_t	i;

	if (is_builtin(in))
		return (0);
	if (ft_strchr(in, '/'))
		return (access(in, X_OK));
	paths = xsplit(ms_getenv("PATH", sh), ':', sh);
	if (!paths)
		return (1);
	i = 0;
	while (paths[i])
	{
		test = xstrjoin3(paths[i], "/", in, sh);
		if (access(test, X_OK) == 0)
			return (ft_strs_clear(paths), 0);
		free(test);
		++i;
	}
	ft_strs_clear(paths);
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Redirect                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief リダイレクトの有効性を検証する
 *
 * @param data リダイレクトトークン
 * @param shell シェル情報
 * @return int 成功時0、失敗時1
 */
int	valid_redir(t_lexical_token *d, t_shell *sh)
{
	int	fd;

	(void)sh;
	if (d->type == TT_REDIR_IN)
		fd = open(d->value, O_RDONLY);
	else if (d->type == TT_REDIR_OUT)
		fd = open(d->value, O_WRONLY | O_CREAT, 0644);
	else if (d->type == TT_APPEND)
		fd = open(d->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (d->type == TT_HEREDOC)
		fd = open(d->value, O_WRONLY | O_CREAT, 0644);
	else
		return (0);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Main Processing function                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief 引数トークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param count 引数の位置（0はコマンド）
 * @param shell
 * @return int 成功時0、失敗時1
 */
int	proc_argv(t_list **list, t_lexical_token *data, int count, t_shell *shell)
{
	char	*aft_env;
	char	*aft_wlc;

	if (!data || !data->value)
		return (1);
	aft_env = handle_env(data->value, shell);
	if (!aft_env)
		return (1);
	if (!ft_strchr(aft_env, ' ')) // スペースで区切られた引数の処理
		;                         // ここに空白区切りであたらしく引数リストを構成する関数を置く
	aft_wlc = handle_wildcard(aft_env, shell);
	if (aft_wlc && !ft_strchr(aft_wlc, ' '))
		; // コマンドの方はワイルドカードのあとに引数をまた構成する
	if (!aft_wlc)
		return (free(aft_env), 1);
	if (count == 0) // コマンド解決（最初の引数の場合）
	{
		if (resolve_path(aft_wlc, shell))
			return (free(aft_env), 1);
	}
	free(data->value); // トークン値の更新
	data->value = aft_wlc;
	return (free(aft_env), 0);
}

/**
 * @brief リダイレクトトークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param count 引数の位置
 * @param shell
 * @return int 成功時0、失敗時1
 */
int	proc_redr(t_list **list, t_lexical_token *data, int count, t_shell *shell)
{
	char	*aft_env;
	char	*aft_wlc;

	if (!data || !data->value)
		return (1);
	/* 環境変数展開 */
	aft_env = handle_env(data->value, shell);
	if (!aft_env)
		return (1);
	if (!ft_strchr(aft_env, ' '))
		; // ここに空白区切りであたらしく引数リストを構成する関数を置く
	aft_wlc = handle_wildcard(aft_env, shell);
	if (!aft_wlc)
		return (free(aft_env), 1);
	if (valid_redir(data, shell))
		return (free(aft_env), free(aft_wlc), 1);
	free(data->value);
	data->value = aft_wlc;
	return (free(aft_env), 0);
}

/**
 * @brief 抽象構文木からコマンドを生成する
 *
 * @param ast
 * @param shell
 * @return int 成功時0、失敗時1
 */
int	ast2cmds(t_ast *ast, t_shell *shell)
{
	int	status;

	status = 0;
	if (ast == NULL)
		return (1);
	if (ast->ntype != NT_CMD)
	{
		status += ast2cmds(ast->left, shell);
		status += ast2cmds(ast->right, shell);
	}
	else
	{
		if (ms_lstiter(ast->args->argv, (void *)proc_argv, shell))
			return (1);
		if (ms_lstiter(ast->args->redr, (void *)proc_redr, shell))
			return (1);
	}
	return (status);
}

t_status	mod_sem(t_shell *shell)
{
	t_ast	*ast;

	ast = shell->ast;
	if (ast2cmds(ast, shell))
	{
		shell->status = 1;
		return (E_SYNTAX);
	}
	if (shell->debug & DEBUG_SEM)
		debug_print_sem(ast, shell);
	shell->status = 0;
	return (E_NONE);
}
