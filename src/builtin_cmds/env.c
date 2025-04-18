/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 00:16:12 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__env(int argc, char **argv, t_shell *sh)
{
	t_list	*lst;

	
	if (argc != 1)
		return (ft_dprintf(2, "minishell: env: no option supported\n"), 1);
	lst = sh->env_map;
	while (lst)
	{
		printf("%s\n", (char *)lst->data);
		lst = lst->next;
	}
	return (0);
}
