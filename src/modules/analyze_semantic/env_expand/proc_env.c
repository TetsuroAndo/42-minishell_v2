/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:12:11 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 20:08:50 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief 変数名を検出し、展開結果をバッファに追加する
 *
 * @param buf 		結果を格納するバッファへのポインタ
 * @param in 		変数名を含む入力文字列
 * @param shell 	シェル情報
 * @return size_t	変数名inを何文字進めたか
 */
static size_t	extract_varname(char **buf, char *in, t_shell *sh)
{
	size_t	klen;
	char	*key;
	char	*val;

	klen = 1;
	if (sh->debug & DEBUG_SEM)
		ft_dprintf(STDERR_FILENO, "[EXPAND_VAR]: %s [POINTER]: %p\n", in, in);
	while (ft_isalnum_under(in[klen]))
		++klen;
	key = ms_substr_gcli(in, 0, klen, sh);
	if (!key)
		return (0);
	if (!key[0])
		return (0);
	val = ms_getenv_gcli(key, sh);
	if (!val)
		val = ms_strdup_gcli("", sh);
	if (key && key[0] && key[1] == '\0')
	{
		*buf = ms_strjoin_gcli(*buf, val, sh);
		ft_gc_untrack(val, sh->gcli);
	}
	else
		*buf = ms_strjoin_gcli(*buf, val, sh);
	return (klen);
}

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
		if (*in == '$')
			in += extract_varname(&s.buf, in + 1, sh) + 1;
	}
	return (s.buf);
}

static char	*shift_or_true(t_list **list, t_lexical_token *tok, int idx, t_shell *sh)
{
	char			*buf;
	t_list			*to_del;

	while (42)
	{
		buf = handle_env(tok->value, sh);
		if (!buf || *buf != '\0')
			break ;
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
				buf = ms_strdup_gcli("true", sh);
			else
				buf = ms_strdup_gcli("", sh);
			break ;
		}
	}
	return (ms_strdup(buf, sh));
}

t_status	proc_env(t_list **list, int idx, t_shell *sh)
{
	t_lexical_token *token;
	char 			*expanded_value;
	
	if (!list || !*list)
		return (E_SYSTEM);
	token = (t_lexical_token *)(*list)->data;
	if (!token || !token->value)
		return (E_SYSTEM);
	expanded_value = shift_or_true(list, token, idx, sh);
	if (!expanded_value)
		return (E_SYSTEM);
	xfree((void **)&token->value);
	token->value = expanded_value;
	return (E_NONE);
}
