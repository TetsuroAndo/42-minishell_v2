/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:39 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 16:21:46 by teando           ###   ########.fr       */
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

static int	delimiter_is_quoted(const char *s)
{
	while (*s)
		if (*s == '\'' || *s == '"')
			return (1);
		else
			++s;
	return (0);
}

/*
** heredoc:
**  - tmpfile を作成し、ユーザが delimiter を打つまで readline
**  - delimiter が quote されていなければ $ 展開をかける
**  - 完了後、tok を < (RELLIR_IN) と同じ扱いに書き換える
*/
static int	handle_heredoc(t_lexical_token *tok, t_shell *sh)
{
	char	tmpl[] = "/tmp/ms_hd_XXXXXX";
	int		fd;
	char	*line;
	int		quoted;
	char	*exp;

	fd = mkstemp(tmpl);
	quoted = delimiter_is_quoted(tok->value);
	if (fd == -1)
		return (ft_dprintf(2, "minishell: heredoc: cannot create tmp\n"), 1);
	while (1)
	{
		line = readline("> ");
		if (!line) /* Ctrl‑D */
			break ;
		if (!ft_strcmp(line, tok->value)) /* delimiter 到達 */
		{
			free(line);
			break ;
		}
		if (!quoted) /* 展開あり */
		{
			exp = handle_env(line, sh);
			write(fd, exp, ft_strlen(exp));
			free(exp);
		}
		else /* quote 付き delimiter: そのまま書く */
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	/* token の種別 / value を < に置換 */
	free(tok->value);
	tok->value = ms_strdup(tmpl, sh);
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
		tok = xmalloc(1, sh);
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
	/* 入っていない → そのまま置換してパス解決 */
	if (idx == 0 && path_resolve(&value, sh))
		return (free(value), 1);
	free(data->value);
	data->value = value;
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
	char	*expanded_value;
	char	*env_exp;
	char	*wc_exp;

	if (!data || !data->value)
		return (1);
	env_exp = handle_env(data->value, sh);
	if (!env_exp)
		return (1);
	wc_exp = handle_wildcard(env_exp, sh);
	free(env_exp);
	if (!wc_exp)
		return (1);
	if (!ft_strchr(wc_exp, ' '))
		return (process_simple_token(data, wc_exp, idx, sh));
	return (process_split_token(list, data, wc_exp, idx, sh));
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

	(void)count;
	if (!data || !data->value)
		return (1);
	if (data->type == TT_HEREDOC)
		return (handle_heredoc(data, sh)); // heredoc は専用ルートで処理する
	aft_env = handle_env(data->value, sh);
	if (!aft_env || *aft_env == '\0')
		return (ft_dprintf(2, "minishell: ambiguous redirect\n"), 1);
	aft_wlc = handle_wildcard(aft_env, sh);
	free(aft_env);
	if (!aft_wlc || *aft_wlc == '\0' || ft_strchr(aft_wlc, ' '))
		return (ft_dprintf(2, "minishell: %s: ambiguous redirect\n", aft_wlc),
			free(aft_wlc), 1);
	if (valid_redir(data, sh))
		return (free(aft_wlc), 1);
	free(data->value);
	data->value = aft_wlc;
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
