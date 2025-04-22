/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:38:31 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 14:00:15 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_lex.h"

/**
 * @brief 連続したリダイレクト記号の数をカウントする
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param symbol カウントする記号
 * @return int 連続した記号の数
 */
static int	count_consecutive_symbols(const char *line, size_t pos, char symbol)
{
	int	count;

	count = 0;
	while (line[pos] && line[pos] == symbol)
	{
		count++;
		pos++;
	}
	return (count);
}

/**
 * @brief リダイレクト後の記号のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param shell シェル情報
 * @return int 0: エラーなし, 1: エラーあり
 */
static int	check_redirect_symbol_error(const char *line, size_t *pos, t_shell *shell)
{
	size_t	temp_pos;
	int		count;
	t_token_type	op;

	temp_pos = *pos + 1;
	skip_spaces(line, &temp_pos);

	if (line[temp_pos] == '|')
	{
		count = count_consecutive_symbols(line, temp_pos, line[temp_pos]);
		if (count > 1)
			print_error_message(TT_OR_OR, count);
		else
			print_error_message(TT_PIPE, count);
		shell->status = E_SYNTAX;
		return (1);
	}
	else if (line[temp_pos] == '&')
	{
		count = count_consecutive_symbols(line, temp_pos, line[temp_pos]);
		if (count > 1)
			print_error_message(TT_AND_AND, count);
		else
			print_error_message(TT_PIPE, 1);
		shell->status = E_SYNTAX;
		return (1);
	}
	else if (line[temp_pos] == ';')
	{
		count = count_consecutive_symbols(line, temp_pos, line[temp_pos]);
		print_error_message(TT_SEMICOLON, count);
		shell->status = E_SYNTAX;
		return (1);
	}
	else if (line[temp_pos] == '(' || line[temp_pos] == ')')
	{
		op = TT_RPAREN;
		if (line[temp_pos] == '(')
			op = TT_LPAREN;
		print_error_message(op, 1);
		shell->status = E_SYNTAX;
		return (1);
	}
	return (0);
}

/**
 * @brief 過剰なリダイレクト記号のエラーチェック
 *
 * @param symbol リダイレクト記号
 * @param count 連続する記号の数
 * @param pos 現在の位置へのポインタ
 * @param shell シェル情報
 * @return int 0: エラーなし, 1: エラーあり
 */
static int	check_excessive_redirect(char symbol, int count, size_t *pos, 
							t_shell *shell)
{
	t_token_type	op;

	op = get_one_char_op(symbol);
	if ((symbol == '<' && count > 2) || (symbol == '>' && count > 2) || 
		(symbol == '|' && count > 2))
	{
		print_error_message(op, count);
		shell->status = E_SYNTAX;
		*pos += count;
		return (1);
	}
	return (0);
}

/**
 * @brief セミコロンと括弧のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param shell シェル情報
 * @return int 0: エラーなし, 1: エラーあり
 */
static int	validate_semicolon_paren(const char *line, size_t *pos, t_shell *shell)
{
	t_token_type	op;
	int				count;
	char			symbol;

	op = get_one_char_op(line[*pos]);
	if (op != TT_SEMICOLON && op != TT_LPAREN && op != TT_RPAREN)
		return (0);
	symbol = line[*pos];
	count = count_consecutive_symbols(line, *pos, symbol);
	if (symbol == ';')
	{
		print_error_message(op, count);
		shell->status = E_SYNTAX;
		*pos += count;
		return (1);
	}
	else if (symbol == '(' || symbol == ')')
	{
		print_error_message(op, 1);
		shell->status = E_SYNTAX;
		*pos += count;
		return (1);
	}
	return (0);
}

/**
 * @brief リダイレクト記号の後に特殊文字が続く場合のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param shell シェル情報
 * @return int 0: エラーなし, 1: エラーあり
 */
int	validate_redirect_special_chars(const char *line, size_t *pos, 
								t_shell *shell)
{
	t_token_type	op;
	int				count;
	char			symbol;

	if (validate_semicolon_paren(line, pos, shell))
		return (1);
	op = get_one_char_op(line[*pos]);
	if (op != TT_REDIR_IN && op != TT_REDIR_OUT && op != TT_PIPE)
		return (0);
	symbol = line[*pos];
	count = count_consecutive_symbols(line, *pos, symbol);
	if (line[*pos] == '>' && count == 1 && line[*pos + 1] == '|')
	{
		print_error_message(TT_PIPE, 1);
		return (shell->status = E_SYNTAX);
	}
	if (line[*pos] == '>' && count == 1 && line[*pos + 1] == '&')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		return (shell->status = E_SYNTAX);
	}
	if ((symbol == '>' || symbol == '<') && count == 1 && check_redirect_symbol_error(line, pos, shell))
		return (1);
	return (check_excessive_redirect(symbol, count, pos, shell));
}

/**
 * @brief リダイレクト記号の後に引数がない場合のエラーチェック
 *
 * @param line 入力行
 * @param pos 現在の位置
 * @param shell シェル情報
 * @return int 0: エラーなし, 1: エラーあり
 */
int	validate_redirect_missing_arg(const char *line, size_t *pos, t_shell *shell)
{
	t_token_type	op;
	size_t			temp_pos;

	printf("arg: %s\n", line);
	op = get_two_char_op(&line[*pos], NULL);
	if (op == TT_ERROR)
		op = get_one_char_op(line[*pos]);
	if (op != TT_REDIR_IN && op != TT_REDIR_OUT && 
		op != TT_APPEND && op != TT_HEREDOC)
		return (0);
	if (op == TT_APPEND || op == TT_HEREDOC)
		temp_pos = *pos + 2;
	else
		temp_pos = *pos + 1;
	skip_spaces(line, &temp_pos);
	if (!line[temp_pos] || get_one_char_op(line[temp_pos]) != TT_ERROR || 
		get_two_char_op(&line[temp_pos], NULL) != TT_ERROR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		shell->status = E_SYNTAX;
		return (1);
	}
	return (0);
}
