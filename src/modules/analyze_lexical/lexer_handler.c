/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:06:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 01:17:03 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief 2文字演算子を識別する
 *
 * @param s 識別する文字列
 * @param len 識別した演算子の長さを格納する変数へのポインタ
 * @return 識別した演算子のトークンタイプ、識別できない場合はTT_ERROR
 */
t_token_type get_two_char_op(const char *s, size_t *len)
{
	if (!s || !s[0] || !s[1])
		return (TT_ERROR);
	if (ft_strncmp(s, ">>", 2) == 0 && len)
	{
		*len = 2;
		return (TT_APPEND);
	}
	if (ft_strncmp(s, "<<", 2) == 0 && len)
	{
		*len = 2;
		return (TT_HEREDOC);
	}
	if (ft_strncmp(s, "&&", 2) == 0 && len)
	{
		*len = 2;
		return (TT_AND_AND);
	}
	if (ft_strncmp(s, "||", 2) == 0 && len)
	{
		*len = 2;
		return (TT_OR_OR);
	}
	return (TT_ERROR);
}

/**
 * @brief 1文字演算子を識別する
 *
 * @param c 識別する文字
 * @return 識別した演算子のトークンタイプ、識別できない場合はTT_ERROR
 */
t_token_type get_one_char_op(char c)
{
	static const t_token_type op_map[128] = {
		['|'] = TT_PIPE,
		['>'] = TT_REDIR_OUT,
		['<'] = TT_REDIR_IN,
		['('] = TT_LPAREN,
		[')'] = TT_RPAREN,
		[';'] = TT_SEMICOLON};

	if (c >= 0 && c < 128 && op_map[(int)c] != 0)
		return (op_map[(int)c]);
	return (TT_ERROR);
}

/**
 * @brief 引用符で囲まれた文字列を読み取る
 *
 * line[pos]から始まる引用符で囲まれた文字列を読み取り、その内容を返す。
 * 引用符の終わりが見つからない場合はエラーとなる。
 *
 * @param line 処理対象の文字列
 * @param pos 現在の位置を示すポインタ（処理後は引用符の次の位置に更新される）
 * @param shell ステータスを保持する構造体
 * @return 引用符の中の文字列。エラー時はNULL
 */
static char *read_quoted_word(const char *line, size_t *pos, t_shell *shell)
{
	char quote;
	size_t start;
	char *content;

	quote = line[*pos];
	start = *pos + 1;
	(*pos)++;
	while (line[*pos] && line[*pos] != quote)
		(*pos)++;
	if (!line[*pos])
	{
		shell->status = E_SYNTAX;
		return (NULL);
	}
	content = ft_substr(line, start, (*pos - start));
	(*pos)++;
	return (content);
}

/**
 * @brief 単語（トークン）を読み取る
 *
 * line[pos]から始まる単語を読み取り、その内容を返す。
 * 単語の終わりは空白文字、演算子、または文字列の終わりで判断される。
 * 引用符で始まる場合は引用符で囲まれた文字列として処理される。
 *
 * @param line 処理対象の文字列
 * @param pos 現在の位置を示すポインタ（処理後は単語の次の位置に更新される）
 * @param shell ステータスを保持する構造体
 * @return 読み取った単語。エラー時はNULL
 */
char *read_word(const char *line, size_t *pos, t_shell *shell)
{
	size_t start;
	char *res;

	if (line[*pos] == '\'' || line[*pos] == '"')
		return (read_quoted_word(line, pos, shell));
	start = *pos;
	while (line[*pos])
	{
		if (ft_isspace(line[*pos]) || get_two_char_op(&line[*pos], NULL) != TT_ERROR || get_one_char_op(line[*pos]) != TT_ERROR)
			break;
		(*pos)++;
	}
	res = ft_substr(line, start, (*pos - start));
	return (res);
}
