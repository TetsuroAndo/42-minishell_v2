/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:55:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 16:37:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/*
** wc_match  ―  '*' だけを扱う最小限のワイルドカードマッチャ
** 返り値 : 1 = 一致, 0 = 不一致
**
** 仕様
** ───────────────────────────────────────────
**  pat : パターン。'*' は「0 文字以上の任意列」を表す
**  str : 対象文字列
**
**  ・'*' は何個でも、先頭/末尾/連続して出現してもよい
**  ・大文字小文字は区別（シェルの pathname 展開と同じ）
**  ・バックスラッシュによるエスケープは実装していない
*/
int	wc_match(const char *pat, const char *str)
{
	/* パターンの末尾に達したら、文字列も終端で一致 */
	if (*pat == '\0')
		return (*str == '\0');
	if (*pat == '*') // '*' に遭遇したら、連続する '*' をまとめてスキップ
	{
		while (*pat == '*') // 複数の '*' をまとめてスキップ
			++pat;
		if (*pat == '\0') // pat が "*…\0" だけだった場合は残り全部にマッチ
			return (1);
		while (*str) // str の各位置で再帰的に照合していく
		{
			if (wc_match(pat, str)) // pat 以降と str 以降がマッチすれば成功
				return (1);
			++str; // 1 文字ずらして再試行
		}
		return (0); // どの位置でもマッチしなければ失敗
	}
	if (*str && *pat == *str)
		return (wc_match(pat + 1, str + 1));
	return (0);
}

static int	append_entry(char **buf, char *entry, char *pat, t_shell *sh)
{
	char	*dup;

	if (entry[0] == '.' && pat[0] != '.')
		return (0);
	dup = ms_strdup(entry, sh);
	*buf = xstrjoin_free2(*buf, dup, sh);
	*buf = xstrjoin_free(*buf, " ", sh);
	return (1);
}

/**
 * @brief ワイルドカード (*) 展開
 *
 * @param buf   追加先バッファへのポインタ
 * @param in    '*' を含む入力位置
 * @param sh    シェル情報
 * @retval size_t 処理した文字数
 */
size_t	extract_wildcard(char **buf, char *in, t_shell *sh)
{
	int	m;
	int	i;

	m = strlen(p);
	i = 1;
	while (i <= m)
	{
		update_first_cell(p[i - 1], prev, curr);
		process_row_cells(p[i - 1], s, prev, curr);
		swap_rows(&prev, &curr);
		i++;
	}
}

int	wildcard_match(const char *p, const char *s)
{
	int	m;
	int	n;
	int	*prev;
	int	*curr;
	int	result;

	m = ft_strlen(p);
	n = ft_strlen(s);
	if (is_invalid_input(p, s))
		return (0);
	prev = init_dp_row(n);
	curr = init_dp_row(n);
	if (!prev || !curr)
		return (0);
	prev[0] = 1;
	update_dp_row(p, s, prev, curr);
	if (m % 2 == 0)
		result = prev[n];
	else
		result = curr[n];
	free(prev);
	free(curr);
	return (result);
}

static char	*append_match(char *buf, const char *name, t_shell *sh)
{
	char	*new_buf;

	if (!buf)
		return (ms_strdup(name, sh));
	new_buf = ft_strjoin3(buf, " ", name);
	free(buf);
	return (new_buf);
}

/**
 * @brief 単語の境界を探し、ワイルドカードの有無を確認する
 *
 * @param in 入力文字列
 * @param has_wc ワイルドカードの有無を格納する変数へのポインタ
 * @param s セマンティック情報
 * @return size_t 単語の長さ
 */
static size_t	find_word_boundary(char *in, int *has_wc, t_sem *s)
{
	size_t	i;

	i = 0;
	s->quote_state = QS_NONE;
	*has_wc = 0;
	while (check_qs(in[i], s) && !ft_isspace(in[i]))
	{
		if (s->quote_state == QS_NONE && in[i] == '*')
			*has_wc = 1;
		++i;
	}
	return (i);
}

/**
 * @brief 単語を処理し、必要に応じてワイルドカード展開を行う
 *
 * @param buf バッファへのポインタ
 * @param in 入力文字列
 * @param word_len 単語の長さ
 * @param has_wc ワイルドカードの有無
 * @param sh シェル情報
 * @return char* 処理後の入力位置
 */
static char	*process_word(char **buf, char *in, size_t word_len, int has_wc,
		t_shell *sh)
{
	size_t	bufl;

	if (has_wc)
	{
		bufl = extract_wildcard(buf, in, sh);
		if (bufl > 0 && (*buf)[bufl - 1] == ' ')
			(*buf)[bufl - 1] = '\0';
	}
	else
		*buf = xstrjoin_free2(*buf, ms_substr(in, 0, word_len, sh), sh);
	return (in + word_len);
}

char	*handle_wildcard(char *in, t_shell *sh)
{
	DIR		*dir;
	char	*buf;

	if (!in || !sh)
		return (NULL);
	if (!strchr(in, '*'))
		return (in);
	dir = opendir(sh->cwd);
	if (!dir)
		return (in);
	buf = collect_matches(dir, in, sh);
	closedir(dir);
	if (buf)
		return (buf);
	else
		return (in);
}
