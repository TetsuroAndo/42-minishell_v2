/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:11:39 by teando            #+#    #+#             */
/*   Updated: 2025/04/24 17:58:39 by tomsato          ###   ########.fr       */
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
	if (!key)
		return (0);
	if (!key[0])
		return (xfree((void **)&key), 0);
	val = ms_getenv(key, sh);
	if (!val)
		val = ms_strdup("", sh);
	if (key && key[0] && key[1] == '\0')
		*buf = xstrjoin_free(*buf, val, sh);
	else
	{
		*buf = xstrjoin_free(*buf, val, sh);
		xfree((void **)&val);
	}
	xfree((void **)&key);
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
		if (*in == '$' && in[1] == '(' && s.quote_state != QS_SINGLE)
		{
			s.buf = xstrjoin_free2(s.buf, ms_substr(in, 0, 2, sh), sh);
			in += 2;
		}
		else if (*in == '$' && s.quote_state != QS_SINGLE)
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
	if (d->type == TT_REDIR_IN && ft_strchr(d->value, '\n'))
		return (0);
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
	char	*delim;

	*quoted = is_quoted(delim_raw);
	delim = trim_valid_quotes(delim_raw, sh);
	xfree((void **)&delim_raw);
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
	char	*q_body;

	q_body = NULL;
	body = ms_strdup("", sh);
	while (42)
	{
		line = read_command_line("> ");
		if (!line)
		{
			if (g_signal_status != SIGINT)
				ft_dprintf(STDERR_FILENO,
					"minishell: warning: here-document delimited by end-of-file\n");
			xfree((void **)&line);
			break ;
		}
		if (g_signal_status == SIGINT)
		{
			sh->status = E_SIGINT;
			return (xfree((void **)&body), xfree((void **)&line), NULL);
		}
		if (!line || (delim[0] == '\0' && line[0] == '\0') || ft_strcmp(line,
				delim) == 0)
		{
			xfree((void **)&line);
			break ;
		}
		body = xstrjoin_free2(body, line, sh);
		body = xstrjoin_free(body, "\n", sh);
	}
	if (quoted)
	{
		q_body = ft_strjoin3("\'", body, "\'");
		return (xfree((void **)&body), q_body);
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
	if (!body)
		body = ms_strdup("", sh);
	xfree((void **)&delim);
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
	t_list			*next;
	t_lexical_token	*tok;
	size_t			i;

	next = (*list)->next;
	(*list)->next = NULL;
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
	if (next)
		ft_lstadd_back(list, next);
	return (0);
}

/**
 * @brief トークンの値をクォート処理して更新する
 *
 * @param data トークンデータ
 * @param val 処理する文字列
 * @param sh シェル情報
 * @return char* 処理後の文字列
 */
static char	*update_token_value(t_lexical_token *data, char *val, t_shell *sh)
{
	char	*trimmed;

	trimmed = trim_valid_quotes(val, sh);
	if (trimmed != val)
		xfree((void **)&val);
	xfree((void **)&data->value);
	data->value = trimmed;
	return (trimmed);
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
static int	process_simple_token(t_lexical_token *data, char *val, int idx,
		t_shell *sh)
{
	if (sh->debug & DEBUG_SEM)
		printf("[process_simple_token] value: %s\n", val);
	update_token_value(data, val, sh);
	if (idx == 0)
		return (path_resolve(&data->value, sh));
	return (E_NONE);
}


/**
 * @brief 空白を含む文字列を分割して処理する
 *
 * @param list トークンリスト
 * @param data トークンデータ
 * @param value 処理する文字列
 * @param idx 引数���位置���0�����コマン���）
 * @param sh シェル情報
 * @return int 成功時0、失敗時1
 */
static int	process_split_token(t_list **list, char *value, int idx,
		t_shell *sh)
{
	char			**words;
	t_lexical_token	*data;
	int				status;

	if (!list || !*list)
		return (xfree((void **)&value), 1);
	data = (t_lexical_token *)(*list)->data;
	if (!data)
		return (xfree((void **)&value), 1);
	if (sh->debug & DEBUG_SEM)
		printf("[process_split_token] value: %s\n", value);
	words = split_with_quote(value, sh);
	if (!words || !words[0])
		return (xfree((void **)&value), ft_strs_clear(words), 1);
	xfree((void **)&value);
	xfree((void **)&data->value);
	data->value = ms_strdup(words[0], sh);
	if (add_to_list(list, words, sh))
		return (ft_strs_clear(words), 1);
	if (idx == 0)
	{
		status = path_resolve(&data->value, sh);
		if (status != E_NONE)
			return (ft_strs_clear(words), status);
	}
	ft_strs_clear(words);
	return (0);
}

/**
 * @brief 入力文字列に含まれるトークン（単語）の数を数える
 *
 * - 空白（スペース・タブ・改行など）で区切られた非空白シーケンスを１トークンとする
 * - シングルクォート(')またはダブルクォート(")で囲まれた部分は、
 *   中の空白を含めて１トークンと扱う
 *
 * @param s 入力文字列（NULL終端）
 * @return size_t トークンの数
 */
size_t	count_aft_wc_tok(char *s)
{
	size_t	count;
	char	*p;
	char	q;

	count = 0;
	p = s;
	while (*p)
	{
		while (*p && isspace((unsigned char)*p))
			p++;
		if (!*p)
			break ;
		if (*p == '"' || *p == '\'')
		{
			q = *p++;
			while (*p && *p != q)
				p++;
			if (*p == q)
				p++;
			count++;
		}
		else
		{
			count++;
			while (*p && !ft_isspace((unsigned char)*p) && *p != '"'
				&& *p != '\'')
				p++;
		}
	}
	return (count);
}

static char	*shift_or_true(t_list **list, int idx, t_shell *sh)
{
	t_lexical_token	*tok;
	char			*buf;
	t_list			*to_del;

	while (42)
	{
		tok = (t_lexical_token *)(*list)->data;
		buf = handle_env(tok->value, sh);
		if (!buf || *buf != '\0')
			break ;
		xfree((void **)&buf);
		if ((*list)->next)
		{
			to_del = (*list)->next;
			(*list)->data = copy_token(to_del->data, sh);
			(*list)->next = to_del->next;
			ft_lstdelone(to_del, free_token);
		}
		else
		{
			if (idx == 0)
				buf = ms_strdup("true", sh);
			else
				buf = ms_strdup("", sh);
			break ;
		}
	}
	return (buf);
}

/* ===== 4. public: argv 用メイン関数 ============================ */
int	proc_argv(t_list **list, t_lexical_token *data, int idx, t_shell *sh)
{
	char	*env_exp;
	char	*wc_exp;
	int		space_count;

	if (!data || !data->value)
		return (1);
	env_exp = shift_or_true(list, idx, sh);
	if (!env_exp)
		return (1);
	wc_exp = handle_wildcard(env_exp, sh);
	if (!wc_exp)
		return (1);
	if (!ft_strchr(wc_exp, ' '))
		return (process_simple_token((*list)->data, wc_exp, idx, sh));
	space_count = count_aft_wc_tok(wc_exp);
	if (process_split_token(list, wc_exp, idx, sh))
		return (1);
	while (space_count-- > 1 && *list && (*list)->next)
		*list = (*list)->next;
	return (0);
}

int	proc_redr_errs(t_lexical_token *data, t_shell *shell)
{
	if (!data->value)
		return (E_SYSTEM);
	if (!ft_strchr(data->value, '\n'))
		return (E_NONE);
	if (*data->value == '\0' || ft_strchr(data->value, ' '))
		return (ft_dprintf(2, "minishell: ambiguous redirect\n"),
			E_AMBIGUOUS_REDIR);
	if (valid_redir(data, shell))
		return (ft_dprintf(2, ES_PERMISSION, data->value), E_PERMISSION_DENIED);
	return (E_NONE);
}

/**
 * @brief リダイレクトトー���ンを処理する
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
	(void)list;
	aft_env = handle_env(data->value, sh);
	if (!aft_env || *aft_env == '\0')
		return (0);
	aft_wlc = handle_wildcard(aft_env, sh);
	if (!aft_wlc)
		return (0);
	aft_unq = replace_with_unquoted(aft_wlc, sh);
	if (aft_unq != aft_wlc)
		xfree((void **)&aft_wlc);
	if (!ft_strchr(aft_unq, '\n') && (!aft_unq || *aft_unq == '\0'
			|| ft_strchr(aft_unq, ' ')))
		return (0);
	xfree((void **)&data->value);
	data->value = aft_unq;
	return (0);
}

static void	del_nul_node(t_list **list)
{
	t_list			*prev;
	t_list			*cur;
	t_lexical_token	*tok;

	prev = NULL;
	cur = *list;
	while (cur)
	{
		tok = (t_lexical_token *)cur->data;
		if (tok->value && tok->value[0] == '\0')
			break ;
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return ;
	if (prev)
		prev->next = cur->next;
	else
		*list = cur->next;
	ft_lstdelone(cur, free_token);
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
		return (0);
	if (ast->ntype != NT_CMD)
	{
		status = ast2cmds(ast->left, shell);
		if (status != E_NONE)
			return (status);
		status = ast2cmds(ast->right, shell);
		if (status != E_NONE)
			return (status);
	}
	else
	{
		status = ms_lstiter(ast->args->argv, (void *)proc_argv, shell);
		if (status != E_NONE)
			return (status);
		del_nul_node(&ast->args->argv);
		status = ms_lstiter(ast->args->redr, (void *)proc_redr, shell);
		if (status != E_NONE)
			return (status);
	}
	return (status);
}

/**
 * @brief redrのHEREDOCだけ先に処理し切る
 */
static int	proc_all_heredocs(t_list *redr, t_shell *shell)
{
	t_list			*p_redr;
	t_lexical_token	*tok;

	p_redr = redr;
	while (p_redr && p_redr->data)
	{
		tok = (t_lexical_token *)(p_redr)->data;
		if (tok->type == TT_HEREDOC)
		{
			if (!tok->value || handle_heredoc(tok, shell))
				return (1);
		}
		p_redr = p_redr->next;
	}
	return (0);
}
/**
 * @brief 単一のASTノードのargs（argv, redr）をバックアップ・復元する
 *
 * @param ast バックアップするASTノード
 * @param shell シェル情報
 */
static void	backup_node_args(t_ast *ast, t_shell *shell)
{
	t_args	*ast_args;

	if (!ast || !ast->args)
		return ;
	ast_args = ast->args;
	if (ast_args->b_argv == NULL)
		ast_args->b_argv = ms_lstcopy(ast_args->argv, free_token, shell);
	if (ast_args->b_redr == NULL)
	{
		proc_all_heredocs(ast_args->redr, shell);
		ast_args->b_redr = ms_lstcopy(ast_args->redr, free_token, shell);
	}
	ft_lstclear(&ast_args->argv, free_token);
	ft_lstclear(&ast_args->redr, free_token);
	ast_args->argv = ms_lstcopy(ast_args->b_argv, free_token, shell);
	ast_args->redr = ms_lstcopy(ast_args->b_redr, free_token, shell);
}

/**
 * @brief AST全体を再帰的に走査して各ノードのバックアップを作成・復元する
 *
 * @param ast バックアップするAST
 * @param shell シェル情報
 */
void	astlst_backup(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return ;
	backup_node_args(ast, shell);
	if (ast->left)
		astlst_backup(ast->left, shell);
	if (ast->right)
		astlst_backup(ast->right, shell);
}

t_status	mod_sem(t_shell *shell)
{
	t_ast		*ast;
	t_status	status;

	ast = shell->ast;
	astlst_backup(ast, shell);
	status = ast2cmds(ast, shell);
	if (status != E_NONE)
	{
		shell->status = status;
		return (status);
	}
	if (shell->debug & DEBUG_SEM)
		debug_print_sem(ast, shell);
	shell->status = 0;
	return (E_NONE);
}
