/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlstnew_gc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:41:29 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 20:42:44 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_list	*xlstnew_gcli(void *data, t_shell *sh)
{
	t_list	*node;

	node = (t_list *)xmalloc_gcline(sizeof(t_list), sh);
	if (!node)
		shell_exit(sh, E_SYSTEM);
	node->data = data;
	node->next = NULL;
	return (node);
}
