/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlstnew.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:29:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 17:30:46 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"

t_list	*xlstnew(void *data, t_shell *shell)
{
	t_list	*list;

	list = ft_lstnew(data);
	if (!list)
		shell_exit(shell, E_SYSTEM);
	return (list);
}
