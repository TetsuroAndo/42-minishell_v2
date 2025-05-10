/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:12:11 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 23:55:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief バックスラッシュと$記号を処理し、環境変数を展開する
 *
 * @param in 処理する入力文字列
 * @param shell シェル情報
 * @return char* 展開後の文字列
 */
static char	*handle_env(char *in, t_shell *sh)
{
	t_expenv	s;
	size_t		i;

	s.buf = ms_strdup_gcli("", sh);
	s.quote_state = QS_NONE;
	while (*in)
	{
		i = 0;
		while (check_qs(in[i], &s) && ((in[i] == '$' && in[i + 1] == '(')
				|| in[i] != '$' || s.quote_state == QS_SINGLE))
			++i;
		s.buf = ms_strjoin_gcli(s.buf, ms_substr_gcli(in, 0, i, sh), sh);
		in += i;
		if (*in == '$' && in[1] && !(in[1] == '\'' || in[1] == '"'
				|| ft_isspace(in[1])))
			in += extract_varname(&s.buf, in + 1, sh) + 1;
		else if (*in == '$')
		{
			s.buf = ms_strjoin_gcli(s.buf, "$", sh);
			++in;
		}
	}
	return (s.buf);
}

static t_status	replace_with_next(t_list **list, t_shell *sh)
{
	t_list	*to_del;

	if (!list || !*list || !(*list)->next)
		return (E_SYSTEM);
	free_token((*list)->data);
	to_del = (*list)->next;
	(*list)->data = copy_token(to_del->data, sh);
	(*list)->next = to_del->next;
	ft_lstdelone(to_del, free_token);
	return (E_NONE);
}

static char	*shift_or_true(t_list **list, int idx, t_shell *sh)
{
	char	*buf;
	char	*ret;

	while (42)
	{
		buf = handle_env(((t_lexical_token *)((*list)->data))->value, sh);
		if (!buf || *buf != '\0')
			break ;
		if ((*list)->next)
		{
			if (replace_with_next(list, sh) != E_NONE)
				return (NULL);
		}
		else
		{
			if (idx == 0)
				buf = ms_strdup_gcli("true", sh);
			else
				buf = ms_strdup_gcli("", sh);
			break ;
		}
	}
	ret = ms_strdup(buf, sh);
	ft_gc_free(sh->gcli, (void **)&buf);
	return (ret);
}

t_status	proc_env(t_list **list, int idx, t_shell *sh)
{
	t_lexical_token	*token;
	char			*expanded_value;

	if (!list || !*list)
		return (E_SYSTEM);
	token = (t_lexical_token *)(*list)->data;
	if (!token || !token->value)
		return (E_SYSTEM);
	if (token->type == TT_HEREDOC)
		expanded_value = heredoc_env(token->value, sh);
	else if (token->type == TT_HEREDOC_NOEXP)
		expanded_value = ms_strdup(token->value, sh);
	else if ((token->type & TM_TYPE) == TM_REDIR)
		expanded_value = ms_strdup(handle_env(token->value, sh), sh);
	else
		expanded_value = shift_or_true(list, idx, sh);
	if (!expanded_value)
		return (E_SYSTEM);
	token = (t_lexical_token *)(*list)->data;
	xfree((void **)&token->value);
	token->value = expanded_value;
	return (E_NONE);
}
