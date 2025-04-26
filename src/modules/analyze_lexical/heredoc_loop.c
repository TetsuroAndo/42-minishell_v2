/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:08:24 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 19:21:42 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

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
				ft_dprintf(STDERR_FILENO, ES_HEREDOC);
			xfree((void **)&line);
			break ;
		}
		if (g_signal_status == SIGINT)
		{
			sh->status = E_SIGINT;
			return (xfree((void **)&body), xfree((void **)&line), NULL);
		}
		if (!line || (delim[0] == '\0' && line[0] == '\0') || !ft_strcmp(line, delim))
		{
			xfree((void **)&line);
			break ;
		}
		body = xstrjoin_free2(body, line, sh);
		body = xstrjoin_free(body, "\n", sh);
	}
	if (quoted)
	{
		q_body = xstrjoin3("\'", body, "\'", sh);
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
int	handle_heredoc(t_lexical_token *tok, t_shell *sh)
{
	int		quoted;
	char	*delim;
	char	*body;

	delim = prepare_delimiter(tok->value, &quoted, sh);
	body = read_heredoc_body(delim, quoted, sh);
	if (!body)
		body = ms_strdup("", sh);
	xfree((void **)&delim);
	tok->value = body;
	return (0);
}
