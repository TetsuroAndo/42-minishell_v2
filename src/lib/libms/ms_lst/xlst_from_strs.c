/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlst_from_strs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:30:07 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:06:54 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_list	*xlst_from_strs(char **strs, t_shell *shell)
{
	t_list	*list;

	list = ft_list_from_strs(strs);
	if (!list)
		shell_exit(shell, E_SYSTEM);
	return (list);
}
