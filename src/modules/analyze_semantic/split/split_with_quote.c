/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:27:41 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 20:05:19 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

static size_t	fill_token_array(char **result, char *str, size_t count,
		t_shell *sh)
{
	size_t	i;
	char	*p;
	char	*token_start;
	size_t	token_len;

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
	return (i);
}

char	**split_with_quote(char *str, t_shell *sh)
{
	char	**result;
	size_t	count;
	size_t	stored;

	if (!str)
		return (NULL);
	count = count_aft_wc_tok(str);
	result = xmalloc_gcline(sizeof(char *) * (count + 1), sh);
	stored = fill_token_array(result, str, count, sh);
	result[stored] = NULL;
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
