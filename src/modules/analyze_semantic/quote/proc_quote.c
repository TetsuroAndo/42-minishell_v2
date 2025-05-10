/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:03:19 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/10 23:51:25 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	proc_quote(t_list **lst, int index, t_shell *sh)
{
	t_lexical_token	*tok;
	char			*tmp;

	(void)index;
	tok = (*lst)->data;
	if (tok->type == TT_HEREDOC_NOEXP)
	{
		tok->type = TT_HEREDOC;
		tok->value = xstrjoin_free(tok->value, "", sh);
		return (0);
	}
	tmp = replace_with_unquoted(tok->value, sh);
	xfree((void **)&tok->value);
	tok->value = ms_strdup(tmp, sh);
	return (0);
}
