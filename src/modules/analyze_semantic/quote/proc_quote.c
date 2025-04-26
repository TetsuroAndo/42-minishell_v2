/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:03:19 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 20:09:08 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	proc_quote(t_list **lst, int index, t_shell *sh)
{
	t_lexical_token *tok;
	char *tmp;

	tok = *lst->data;
	tmp = replace_with_unquoted(tok->value, sh);
	xfree((void **)&tok->value);
	tok->value = tmp;
	ft_gc_untrack(sh->gcli, tok->value);
	return (0);
}
