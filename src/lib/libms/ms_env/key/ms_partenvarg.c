/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_partenvarg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 00:28:32 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 08:05:24 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_status	ms_partenvarg(char *key, const char *arg)
{
    size_t keylen;

	keylen = ft_strcspn(arg, "=");
    if (keylen >= PATH_MAX)
    {
        return (E_ENV_KEY);
    }
	ft_strlcpy(key, arg, keylen + 1);
    if (!ms_isactivekey(key))
        return (E_ENV_KEY);
    return (E_NONE);
}
