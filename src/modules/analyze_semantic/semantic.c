/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:39 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 11:04:23 by teando           ###   ########.fr       */
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

t_quote_state	is_quote_type(int c)
{
	if (c == '"')
		return (QS_DOUBLE);
	if (c == '\'')
		return (QS_SINGLE);
	if (c == '`')
		return (QS_BACK);
	return (QS_NONE);
}

int	check_qs(int c, t_sem *sem)
{
	if (sem->quote_state == QS_NONE)
		sem->quote_state = is_quote_type(c);
	else if (sem->quote_state == is_quote_type(c))
		sem->quote_state = QS_NONE;
	return (c);
}

/**
 * @brief ワイルドカード（*）を検出し、展開結果をバッファに追加する
 *
 * @param buf		結果を格納するバッファへのポインタ
 * @param in 		'*' を含む入力文字列
 * @param shell		シェル情報
 * @return size_t	inを何文字進めたか
 */
size_t	extract_wildcard(char **buf, char *in, t_shell *shell)
{
	const char	*pwd = shell->cwd;

	//
	// パターンマッチングの関数を作る。
	return (0);
}

/**
 * @brief 入力文字列中のバックスラッシュとワイルドカード(*)を処理し、
 *        展開結果を新しいバッファとして返す
 *
 * @param in 処理する入力文字列
 * @param shell シェル情報
 * @return char* 展開後の文字列
 */
char	*handle_wildcard(char *in, t_shell *shell)
{
	t_sem	sem;
	size_t	i;

	sem.buf = ms_strdup("", shell);
	sem.quote_state = QS_NONE;
	while (*in)
	{
		i = 0;
		while (check_qs(in[i], &sem) && ((!ft_isbackslash(in[i])
					&& in[i] != '*') || sem.quote_state == QS_SINGLE
				|| sem.quote_state == QS_DOUBLE))
			i++;
		sem.buf = xstrjoin_free2(sem.buf, ms_substr(in, 0, i, shell), shell);
		in += i;
		if (*in == '*' && sem.quote_state == QS_NONE)
			in += extract_wildcard(&sem.buf, in, shell);
		else if (*in)
			check_qs(*in++, &sem);
	}
	return (sem.buf);
}

/**
 * @brief 変数名を検出し、展開結果をバッファに追加する
 *
 * @param buf 		結果を格納するバッファへのポインタ
 * @param in 		変数名を含む入力文字列
 * @param shell 	シェル情報
 * @return size_t	変数名inを何文字進めたか
 */
size_t	extract_varname(char **buf, char *in, t_shell *shell)
{
	size_t	key_len;
	char	*key_str;
	char	*env_val;

	key_len = 1; // 0文字目には特殊記号を含むため
	while (ft_isalnum_under(in[key_len]))
		key_len++;
	key_str = ms_substr(in, 0, key_len, shell);
	env_val = ms_getenv(key_str, shell);
	if (!env_val)
		env_val = ms_strdup("", shell);
	*buf = xstrjoin_free2(*buf, env_val, shell);
	xfree(&key_str);
	return (key_len);
}

/**
 * @brief バックスラッシュと$記号を処理し、環境変数を展開する
 *
 * @param in 処理する入力文字列
 * @param shell シェル情報
 * @return char* 展開後の文字列
 */
char	*handle_env(char *in, t_shell *shell)
{
	t_sem	sem;
	size_t	i;

	sem.buf = ms_strdup("", shell);
	sem.quote_state = QS_NONE;
	while (*in)
	{
		i = 0;
		while (check_qs(in[i], &sem) && ((!ft_isbackslash(in[i])
					&& in[i] != '$') || sem.quote_state == QS_SINGLE))
			i++;
		sem.buf = xstrjoin_free2(sem.buf, ms_substr(in, 0, i, shell), shell);
		in += i;
		if (*in == '$' && sem.quote_state != QS_SINGLE)
			in += extract_varname(&sem.buf, in + 1, shell) + 1;
		else if (ft_isbackslash(*in) && in[1] != '*'
			&& sem.quote_state != QS_SINGLE)
		{
			sem.buf = xstrjoin_free2(sem.buf, ms_substr(in + 1, 0, 1, shell),
					shell);
			in += 2;
		}
		else if (*in)
			check_qs(*in++, &sem);
	}
	return (sem.buf);
}

int	resolve_path(char *in, t_shell *shell)
{
	return (0);
}

/**
 * @brief 引数トークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param count 引数の位置（0はコマンド）
 * @param shell シェル情報
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
 * @brief リダイレクトの有効性を検証する
 *
 * @param data リダイレクトトークン
 * @param shell シェル情報
 * @return int 成功時0、失敗時1
 */
int	valid_redir(t_lexical_token *data, t_shell *shell)
{
	return (0);
}

/**
 * @brief リダイレクトトークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param count 引数の位置
 * @param shell シェル情報
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
 * @param ast 抽象構文木
 * @param shell シェル情報
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
		if (ms_lstiter(ast->args->argv, proc_argv, shell))
			return (1);
		if (ms_lstiter(ast->args->redr, proc_redr, shell))
			return (1);
	}
	return ((0 < status));
}

t_status	mod_sem(t_shell *shell)
{
	t_ast	*ast;

	ast = shell->ast;
	if (ast2cmds(ast, shell))
	{
		// エラーハンドリング
		return (shell->status);
	}
	return (E_NONE);
}
