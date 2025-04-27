/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:24:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 21:24:35 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_getenv(const char *key, t_shell *shell)
{
	t_list	*lst;

	if (key[0] && key[1] == '\0' && shell->env_spc[(unsigned char)*key])
		return (ms_strdup(shell->env_spc[(unsigned char)*key], shell));
	lst = ft_list_find(shell->env_map, (void *)key, ms_envcmp);
	if (!lst)
		return (ms_strdup("", shell));
	return (ms_substr_r(lst->data, '=', shell));
}
