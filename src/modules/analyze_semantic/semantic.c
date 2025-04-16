/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/17 08:32:48 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

// wordを引数にコネコネする関数→ft_lstiterみたいなやつで全部に適用したら良さそうかも
// 文字リテラル
// → リダイレクト
// → CMD 解決
//   → ビルドイン
//   → PATH 解決
//     → minishell: %s: command not found
// → 展開可能文字 * $

/*
** ワイルドカード（*）を検出し、展開結果をバッファに追加する
** buf: 結果を格納するバッファへのポインタ
** in: '*' を含む入力文字列
** shell: シェル情報
** 戻り値: inを何文字進めたか
*/
int	extract_wildcard(char **buf, char *in, t_shell *shell)
{
	const char *pwd = shell->cwd;
	//
	//パターンマッチングの関数を作る。
	return 0;
}

/*
** 入力文字列中のバックスラッシュとワイルドカード(*)を処理し、
** 展開結果を新しいバッファとして返す
*/
char	*handle_wildcard(char *in, t_shell *shell)
{
	char	*buf;
	size_t	i;

	buf = ms_strdup("", shell);
	while (*in)
	{
		i = 0;
		while (in[i] && (!ft_isbackslash(in[i]) && in[i] != '*'))
			i++;
		buf = xstrjoin_free2(buf, ms_substr(in, 0, i, shell), shell);
		in += i;
		if (in[i] == '*')
			in += extract_wildcard(&buf, in, shell);
		else if (*in)
			in++;
	}
	return (buf);
}

int	extract_varname(char **buf, char *in, t_shell *shell)
{
	size_t	key_len;
	char	*key_str;
	char	*env_val;

	key_len = 0;
	while (ft_isalnum_under(in[key_len]))
		key_len++;
	if (key_len == 0)
	{
		*buf = xstrjoin_free2(*buf, "$", shell);
		return (1);
	}
	key_str = ms_substr(in, 0, key_len, shell);
	env_val = ms_getenv(key_str, shell);
	if (!env_val)
		env_val = "";
	*buf = xstrjoin_free2(*buf, env_val, shell);
	xfree(&key_str);
	return (key_len);
}

/*
**バックスラッシュと$記号を見つけて適切にバッファーにデータを詰めて返す
*/
char	*handle_env(char *in, t_shell *shell)
{
	char	*buf;
	size_t	i;

	buf = ms_strdup("", shell);
	while (*in)
	{
		i = 0;
		while (in[i] && (!ft_isbackslash(in[i]) && in[i] != '$'))
			i++;
		buf = xstrjoin_free2(buf, ms_substr(in, 0, i, shell), shell);
		in += i;
		if (in[i] == '$')
			in += extract_varname(&buf, in, shell);
		else if (ft_isbackslash(*in) && in[1] != '*')
		{
			buf = xstrjoin_free2(buf, ms_substr(in + 1, 0, 1, shell), shell);
			in += 2;
		}
		else if (*in)
			in++;
	}
	return (buf);
}

int	resolve_path(char *in, t_shell *shell)
{
	return (0);
}

int	proc_argv(t_list **list, t_lexical_token *data, int count, t_shell *shell)
{
	char *aft_env;
	char *aft_wlc;
	
	// 文字リテラル
	if (data->value)
		aft_env = handle_env(data->value, shell);
	if (aft_env)
	{
		if (!ft_strchr(aft_env ,' '))
			;//ここに空白区切りであたらしく引数リストを構成する関数を置く
		aft_wlc = handle_wildcard(data->value, shell);
		if (aft_wlc && !ft_strchr(aft_wlc ,' '))
			;//コマンドの方はワイルドカードのあとに引数をまた構成する
	}
	if (!aft_wlc)
		return (1);
	// CMD 解決
	if (count == 0)
		if (resolve_path(data->value, shell))
			rteurn(1);
	free(data->value);
	free(aft_env);
	data->value = aft_wlc;
	return (0);
}

int	valid_redir(t_lexical_token *data, t_shell *shell)
{
	return (0);
}

int	proc_redr(t_list **list, t_lexical_token *data, int count, t_shell *shell)
{
	char *aft_env;
	char *aft_wlc;

	// 文字リテラル
	if (data->value)
		aft_env = handle_env(data->value, shell);
		if (aft_env)
		{
			if (!ft_strchr(aft_env ,' '))
				;//ここに空白区切りであたらしく引数リストを構成する関数を置く
			aft_wlc = handle_wildcard(data->value, shell);
		}
	if (!aft_wlc)
		return (1);
	// リダイレクト
	valid_redir(data, shell);
	free(data->value);
	free(aft_env);
	data->value = aft_wlc;
	return (0);
}

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
		//エラーハンドリング
		return (shell->status);
	}
	return (E_NONE);
}
