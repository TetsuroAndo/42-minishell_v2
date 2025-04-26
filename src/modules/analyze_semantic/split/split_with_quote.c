/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:27:41 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 22:19:24 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief 次のトークンの開始位置と長さを取得する
 *
 * @param p 探索開始位置
 * @param start トークン開始位置へのポインタ（結果格納用）
 * @return size_t トークンの長さ、トークンがなければ0
 */
static size_t	get_next_token(char **p, char **start)
{
	char	q;
	char	*pos;

	pos = *p;
	while (*pos && isspace((unsigned char)*pos))
		pos++;
	if (!*pos)
		return (0);
	*start = pos;
	if (*pos == '"' || *pos == '\'')
	{
		q = *pos++;
		while (*pos && *pos != q)
			pos++;
		if (*pos == q)
			pos++;
	}
	else
	{
		while (*pos && !isspace((unsigned char)*pos) && *pos != '"'
			&& *pos != '\'')
			pos++;
	}
	*p = pos;
	return (pos - *start);
}

/**
 * @brief split_with_quoteで確保した文字列配列を解放する
 *
 * @param result 解放する文字列配列
 * @param sh シェル情報
 */
void	free_split_result(char **result, t_shell *sh)
{
	size_t	i;

	if (!result)
		return ;
	i = 0;
	while (result[i])
	{
		ft_gc_free(sh->gcli, (void **)&result[i]);
		i++;
	}
	ft_gc_free(sh->gcli, (void **)&result);
}

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

/**
 * @brief 文字列をクォートを考慮して分割する
 *
 * - 空白（スペース・タブ・改行など）で区切る
 * - シングルクォート(')またはダブルクォート(")で囲まれた部分は、
 *   中の空白を含めて１トークンとして扱う
 *
 * @param str 分割する文字列
 * @param sh シェル情報
 * @return char** 分割された文字列の配列（NULLで終端）
 */
char	**split_with_quote(char *str, t_shell *sh)
{
	char	**result;
	size_t	count;
	size_t	i;
	char	*p;
	char	*token_start;
	size_t	token_len;

	if (!str)
		return (NULL);
	count = count_aft_wc_tok(str);
	result = xmalloc_gcline(sizeof(char *) * (count + 1), sh);
	p = str;
	i = 0;
	while (i < count)
	{
		token_len = get_next_token(&p, &token_start);
		if (token_len == 0)
			break ;
		result[i] = ms_substr_gcli(token_start, 0, token_len, sh);
		i++;
	}
	result[i] = NULL;
	return (result);
}

static int	set_first_token(t_lexical_token *token, char **split_tokens,
		t_shell *sh)
{
	if (!split_tokens || !split_tokens[0])
		return (1);
	xfree((void **)&token->value);
	token->value = ms_strdup(split_tokens[0], sh);
	return (0);
}

static t_list	*insert_new_token(t_list *current, const char *value, int type,
		t_shell *sh)
{
	t_lexical_token	*new_token;
	t_list			*new_node;

	new_token = xmalloc_gcline(sizeof(t_lexical_token), sh);
	new_token->type = type;
	new_token->value = ms_strdup_gcli(value, sh);
	new_node = ft_lstnew(new_token);
	if (!new_node)
		shell_exit(sh, E_SYSTEM);
	new_node->next = current->next;
	current->next = new_node;
	ft_gc_untrack(sh->gcli, new_token->value);
	ft_gc_untrack(sh->gcli, new_node->data);
	return (new_node);
}

int	proc_split(t_list **lst, int index, t_shell *sh)
{
	t_list			*current;
	t_lexical_token	*token;
	char			**split_tokens;
	int				i;

	(void)index;
	if (!lst || !*lst)
		return (1);
	current = *lst;
	token = (t_lexical_token *)current->data;
	if (!token)
		return (1);
	split_tokens = split_with_quote(token->value, sh);
	if (set_first_token(token, split_tokens, sh))
		return (1);
	i = 1;
	while (split_tokens[i])
	{
		current = insert_new_token(current, split_tokens[i], token->type, sh);
		i++;
	}
	free_split_result(split_tokens, sh);
	return (0);
}
