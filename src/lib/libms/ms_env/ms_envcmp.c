/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_envcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:21:37 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 09:55:37 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	ms_envcmp(void *data, void *key)
{
	const char		*env_str = (char *)data;
	const size_t	key_len = ft_strlen((char *)key);
	size_t			env_key_len;

	env_key_len = 0;
	while (env_str[env_key_len] && env_str[env_key_len] != '=')
		env_key_len++;
	if (key_len != env_key_len)
		return (1);
	return (ft_strncmp(env_str, (char *)key, key_len));
}
