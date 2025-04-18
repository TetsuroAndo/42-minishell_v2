/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_listshift.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:32:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 23:33:08 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	*ms_listshift(t_list **list)
{
	t_list	*tmp;

	if (!list || !*list)
		return (NULL);
	tmp = *list;
	*list = (*list)->next;
	return (tmp);
}
