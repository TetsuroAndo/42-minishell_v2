/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:49:43 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/16 03:19:54 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"
#include <stddef.h>

int	ms_lstiter(t_list *lst, int (*f)(void *, int, t_shell *), t_shell *shell)
{
	int	i;

	i = 0;
	if (f)
		return (1);
	while (lst != NULL)
	{
		if (f(lst->data, i, shell))
			return (1);
		lst = lst->next;
		i++;
	}
	return (0);
}
