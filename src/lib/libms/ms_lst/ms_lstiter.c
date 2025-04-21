/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:49:43 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/16 21:00:51 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"
#include <stddef.h>

int	ms_lstiter(t_list *lst, int (*f)(t_list **, void *, int, t_shell *), t_shell *shell)
{
	int	ret;
	int	idx;

	idx = 0;
	if (!f)
		return (E_GENERAL);
	while (lst)
	{
		ret = f(&lst, lst->data, idx, shell);
		if (ret != E_NONE)
			return (ret);
		lst = lst->next;
		idx++;
	}
	return (E_NONE);
}
