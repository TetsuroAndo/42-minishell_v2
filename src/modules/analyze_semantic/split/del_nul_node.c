/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_nul_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:00:28 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 20:04:56 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

void	del_nul_node(t_list **list)
{
	t_list			*prev;
	t_list			*cur;
	t_lexical_token	*tok;

	prev = NULL;
	cur = *list;
	while (cur)
	{
		tok = (t_lexical_token *)cur->data;
		if (tok->value && tok->value[0] == '\0')
			break ;
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return ;
	if (prev)
		prev->next = cur->next;
	else
		*list = cur->next;
	ft_lstdelone(cur, free_token);
}
