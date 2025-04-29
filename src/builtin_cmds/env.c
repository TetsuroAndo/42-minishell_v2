/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 19:04:59 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__env(int argc, char **argv, t_shell *sh)
{
	t_list	*lst;
	char	*entry;
	char	*val_ptr;

	(void)argv;
	if (argc != 1)
		return (ft_dprintf(2, "minishell: env: no option supported\n"), 1);
	lst = sh->env_map;
	while (lst && lst->data)
	{
		entry = ms_strdup((char *)lst->data, sh);
		val_ptr = ft_strchr(entry, '=');
		if (val_ptr)
		{
			if (val_ptr[1] == '\0')
				*++val_ptr = '\0';
			printf("%s\n", entry);
		}
		xfree((void **)&entry);
		lst = lst->next;
	}
	return (0);
}
