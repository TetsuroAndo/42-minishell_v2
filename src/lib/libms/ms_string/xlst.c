/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:18:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 20:44:06 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "core.h"

t_list	*xlstnew(void *data, t_shell *shell)
{
	t_list	*list;

	list = ft_lstnew(data);
	if (!list)
		shell_exit(shell, E_ALLOCATE);
	return (list);
}

t_list	*xlst_from_strs(char **strs, t_shell *shell)
{
	t_list	*list;

	list = ft_list_from_strs(strs);
	if (!list)
		shell_exit(shell, E_ALLOCATE);
	return (list);
}

char	**xlst_to_strs(t_list *lst, t_shell *shell)
{
	char	**strs;

	strs = ft_list_to_strs(lst);
	if (!strs)
		shell_exit(shell, E_ALLOCATE);
	return (strs);
}
