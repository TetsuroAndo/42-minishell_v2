/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:15 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:11:32 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/*
 * 演算子タイプ (op) が引数を伴う場合のトークン追加処理。
 * (例: >> file, << LIMITER, > file, < file)
 */
static int	add_operator_token_with_arg(t_token_type op, const char *line,
		size_t *pos, t_shell *shell)
{
	char	*word;

	skip_spaces(line, pos);
	word = read_word(line, pos, shell);
	return (add_token(shell, create_token(op, word, shell)));
}

/*
 * 2文字演算子をパースしてトークンを生成する。
 * 成功した場合は *pos を進め、トークンが必要なら追加して 1 を返す。
 * 該当がない場合は 0 を返す。
 */
static int	parse_two_char_operator(const char *line, size_t *pos,
		t_shell *shell)
{
	t_token_type	op;
	size_t			length;

	op = get_two_char_op(&line[*pos], &length);
	if (op == TT_ERROR)
		return (0);
	(*pos) += length;
	if (op == TT_APPEND || op == TT_HEREDOC)
	{
		if (add_operator_token_with_arg(op, line, pos, shell))
			return (-1);
	}
	else
	{
		if (add_token(shell, create_token(op, NULL, shell)))
			return (-1);
	}
	return (1);
}

/*
 * 1文字演算子をパースしてトークンを生成する。
 * 成功した場合は *pos を進め、トークンが必要なら追加して 1 を返す。
 * 該当がない場合は 0 を返す。
 */
static int	parse_one_char_operator(const char *line, size_t *pos,
		t_shell *shell)
{
	t_token_type	op;

	op = get_one_char_op(line[*pos]);
	if (op == TT_ERROR)
		return (0);
	(*pos)++;
	if (op == TT_REDIR_IN || op == TT_REDIR_OUT)
	{
		if (add_operator_token_with_arg(op, line, pos, shell))
			return (-1);
	}
	else
	{
		if (add_token(shell, create_token(op, NULL, shell)))
			return (-1);
	}
	return (1);
}

/*
 * 次のトークンをパースし、トークンをリストに追加する。
 * return値:
 *   1: トークンを取得した(ループ継続)
 *   0: EOFトークンを追加し終了(ループ終了)
 *  -1: エラー(構文エラーなど) (ループ終了)
 */
static int	parse_next_token(const char *line, size_t *pos, t_shell *shell)
{
	char	*word;
	int r;

	skip_spaces(line, pos);
	if (!line[*pos]) // 入力終端(=EOF)ならTT_EOFトークンを追加して終了
	{
		if (add_token(shell, create_token(TT_EOF, NULL, shell)))
			return (-1);
		return (0);
	}
	if (line[*pos] == '#') // コメント記号(#)が見つかった場合、行の残りをスキップしてEOFトークンを追加
	{
		while (line[*pos])
			(*pos)++;
		if (add_token(shell, create_token(TT_EOF, NULL, shell)))
			return (-1);
		return (0);
	}
	r = parse_two_char_operator(line, pos, shell); // 2文字演算子をチェック
	if (r)
		return (r);
	r = parse_one_char_operator(line, pos, shell); // 1文字演算子をチェック
	if (r)
		return (r);
	word = read_word(line, pos, shell); // いずれの演算子にも該当しない場合は通常単語扱い
	if ((!word && shell->status == E_SYNTAX))// バックスラッシュの時にエラー
		return (-1); // クォート不整合など
	if (add_token(shell, create_token(TT_WORD, word, shell)))
		return (-1);
	return (1);
}

/*
 * 与えられた source_line をトークン列に変換するメイン処理。
 * 成功/失敗をintで返す (1: 成功, 0: エラー)
 */
int	tokenize_line(t_shell *shell)
{
	size_t		index;
	int			parse_status;
	const char	*line = shell->source_line;

	index = 0;
	shell->status = E_NONE;
	while (1)
	{
		parse_status = parse_next_token(line, &index, shell);
		if (parse_status == 0)
			break ; // EOFトークン追加済み -> 正常終了
		if (parse_status < 0) // 構文エラー等
		{
			if (shell->status == E_NONE)
				shell->status = E_SYNTAX;
			return (0);
		}
	}
	return (1);
}
