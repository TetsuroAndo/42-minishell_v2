/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:39 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 15:27:57 by tomsato          ###   ########.fr       */
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
		else if (ft_isbackslash(*in) && s.quote_state != QS_SINGLE)
		{
			if (ft_isbackslash(in[1]) || (in[1] == '*') || (in[1] == '\'')
					|| (in[1] == '"'))
				s.buf = xstrjoin_free2(s.buf, ms_substr(in, 0, 2, sh), sh);
			else
			{
				s.buf = xstrjoin_free2(s.buf, ms_substr(in + 1, 0, 1, sh), sh);
				in += 2;
			}
		}
		else if (*in)
			check_qs(*in++, &s);
	}
	return (s.buf);
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
	else
		return (0);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* heredoc                                                                    */
/* -------------------------------------------------------------------------- */

/**
 * @brief デリミタを準備する
 *
 * @param delim_raw 生のデリミタ文字列
 * @param quoted クォートされているかの情報を格納する変数へのポインタ
 * @param sh シェル情報
 * @return char* 処理済みのデリミタ
 */
static char	*prepare_delimiter(char *delim_raw, int *quoted, t_shell *sh)
{
	char	*delim_noq;
	char	*delim;

	*quoted = is_quoted(delim_raw);
	delim_noq = strip_quotes(delim_raw, sh);
	if (*quoted)
		delim = delim_noq;
	else
		delim = handle_env(delim_noq, sh);
	return (delim);
}

/**
 * @brief ヒアドキュメントの本文を読み込む
 *
 * @param delim デリミタ文字列
 * @param quoted デリミタがクォートされているか
 * @param sh シェル情報
 * @return char* 読み込んだヒアドキュメントの本文
 */
static char	*read_heredoc_body(char *delim, int quoted, t_shell *sh)
{
	char	*body;
	char	*line;

	body = ms_strdup("", sh);
	while (42)
	{
		line = readline("> ");
		if (!line || (delim[0] == '\0' && line[0] == '\0') || ft_strcmp(line,
				delim) == 0)
		{
			free(line);
			break ;
		}
		if (!quoted)
			body = xstrjoin_free2(body, handle_env(line, sh), sh);
		else
			body = xstrjoin_free2(body, line, sh);
		body = xstrjoin_free(body, "\n", sh);
	}
	return (body);
}

/**
 * @brief ヒアドキュメントを処理する
 *
 * @param tok 処理するトークン
 * @param sh シェル情報
 * @return int 成功時0、失敗時1
 */
static int	handle_heredoc(t_lexical_token *tok, t_shell *sh)
{
	char	*delim_raw;
	int		quoted;
	char	*delim;
	char	*body;

	delim_raw = tok->value;
	delim = prepare_delimiter(delim_raw, &quoted, sh);
	body = read_heredoc_body(delim, quoted, sh);
	if (!quoted)
		free(delim);
	tok->value = body;
	tok->type = TT_REDIR_IN;
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Main Processing function                                                   */
/* -------------------------------------------------------------------------- */

int	add_to_list(t_list **list, char **words, t_shell *sh)
{
	t_list			*new;
	t_lexical_token	*tok;
	size_t			i;

	i = 1;
	while (words[i])
	{
		tok = xmalloc(sizeof *tok, sh);
		if (!tok)
			return (1);
		tok->type = TT_WORD;
		tok->value = ms_strdup(words[i++], sh);
		new = xlstnew(tok, sh);
		if (!new)
			return (1);
		ft_lstadd_back(list, new);
	}
	return (0);
}

/**
 * @brief 空白を含まない文字列を処理する
 *
 * @param data トークンデータ
 * @param value 処理する文字列
 * @param idx 引数の位置（0はコマンド）
 * @param sh シェル情報
 * @return int 成功時0、失敗時1
 */
static int	process_simple_token(t_lexical_token *data, char *value, int idx,
		t_shell *sh)
{
	if (idx == 0)
	{
		if (path_resolve(&data->value, sh))
			return (1);
		if (value != data->value)
			free(value);
	}
	else
	{
		free(data->value);
		data->value = value;
	}
	return (0);
}

/**
 * @brief 空白を含む文字列を分割して処理する
 *
 * @param list トークンリスト
 * @param data トークンデータ
 * @param value 処理する文字列
 * @param idx 引数の位置（0はコマンド）
 * @param sh シェル情報
 * @return int 成功時0、失敗時1
 */
static int	process_split_token(t_list **list, t_lexical_token *data,
		char *value, int idx, t_shell *sh)
{
	char	**words;

	words = xsplit(value, ' ', sh);
	if (!words)
		return (free(value), 1);
	free(data->value); // 先頭語で current トークンを上書き
	data->value = ms_strdup(words[0], sh);
	if (add_to_list(list, words, sh)) // 追加トークン(words[1]〜) をリストに連結
		return (ft_strs_clear(words), free(value), 1);
	if (idx == 0 && path_resolve(&data->value, sh)) // 先頭語についてパス解決（ここで初めて呼ぶ
		return (ft_strs_clear(words), free(value), 1);
	ft_strs_clear(words);
	free(value);
	return (0);
}

/**
 * @brief 引数トークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param idx 引数の位置（0はコマンド）
 * @param shell シェル情報
 * @return int 成功時0、失敗時1
 */
int	proc_argv(t_list **list, t_lexical_token *data, int idx, t_shell *sh)
{
	char	*env_exp;
	char	*wc_exp;
	char	*anq_exp;

	if (!data || !data->value)
		return (1);
	env_exp = handle_env(data->value, sh);
	if (!env_exp)
		return (1);
	wc_exp = handle_wildcard(env_exp, sh);
	if (env_exp != wc_exp)
		free(env_exp);
	if (!wc_exp)
		return (1);
	anq_exp = replace_with_unquoted(wc_exp, sh);
	free(wc_exp);
	if (!anq_exp)
		return (1);
	if (!ft_strchr(anq_exp, ' '))
		return (process_simple_token(data, anq_exp, idx, sh));
	return (process_split_token(list, data, anq_exp, idx, sh));
}

/**
 * @brief リダイレクトトークンを処理する
 *
 * @param list トークンリスト
 * @param data 処理するトークンデータ
 * @param idx 引数の位置
 * @param shell シェル情報
 * @return int 成功時0、失敗時1
 */
int	proc_redr(t_list **list, t_lexical_token *data, int count, t_shell *sh)
{
	char	*aft_env;
	char	*aft_wlc;
	char	*aft_unq;

	(void)count;
	if (!data || !data->value)
		return (1);
	if (data->type == TT_HEREDOC)
		return (handle_heredoc(data, sh)); // heredoc は専用ルートで処理する
	aft_env = handle_env(data->value, sh);
	if (!aft_env || *aft_env == '\0')
		return (ft_dprintf(2, "minishell: ambiguous redirect\n"), free(aft_env),
			1);
	aft_wlc = handle_wildcard(aft_env, sh);
	free(aft_env);
	if (!aft_wlc)
		return (ft_dprintf(2, "minishell: ambiguous redirect\n"), 1);
	aft_unq = replace_with_unquoted(aft_wlc, sh);
	free(aft_wlc);
	if (!aft_unq || *aft_unq == '\0' || ft_strchr(aft_unq, ' '))
		return (ft_dprintf(2, "minishell: %s: ambiguous redirect\n",
				aft_unq ? aft_unq : ""), free(aft_unq), 1);
	free(data->value);
	data->value = aft_unq;
	if (valid_redir(data, sh))
		return (free(aft_unq), 1);
	return (0);
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
