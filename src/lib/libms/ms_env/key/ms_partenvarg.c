/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_partenvarg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 00:28:32 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 09:34:49 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_status	ms_partenvarg(char *key, const char *arg, int *is_append,
		size_t *eq_pos)
{
	size_t	keylen;
	char	*plus_pos;

	plus_pos = ft_strstr(arg, "+=");
	if (plus_pos)
	{
		keylen = plus_pos - arg;
		*is_append = 1;
		*eq_pos = keylen + 2;
	}
	else if (ft_strchr(arg, '='))
	{
		keylen = (size_t)(ft_strchr(arg, '=') - arg);
		*eq_pos = keylen + 1;
	}
	else
		keylen = ft_strlen(arg);
	if (keylen >= PATH_MAX || !ft_strlcpy(key, arg, keylen + 1)
		|| !ms_isactivekey(key))
		return (E_ENV_KEY);
	return (E_NONE);
}
