/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xlst.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:18:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 23:31:38 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	**xlst_to_strs(t_list *lst, t_shell *shell)
{
	char	**strs;

	strs = ft_list_to_strs(lst);
	if (!strs)
		shell_exit(shell, E_ALLOCATE);
	return (strs);
}
