/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_isactiveky.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:12:35 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 09:31:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	ms_isactivekey(const char *key)
{
	size_t	i;

	if (!key || !*key)
		return (0);
	if (!ft_isalpha(*key) && *key != '_')
		return (ft_dprintf(STDERR_FILENO,
				"minishell: export: `%s\': not a valid identifier\n", key), 0);
	i = 0;
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (ft_dprintf(STDERR_FILENO,
					"minishell: export: `%s\': not a valid identifier\n", key),
				0);
	}
	return (1);
}
