/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_envcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:21:37 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 00:32:25 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	ms_envcmp(void *data, void *key)
{
	char	ref[PATH_MAX];

	ft_strlcpy(ref, (char *)key, PATH_MAX);
    ft_strlcat(ref, "=", PATH_MAX);
	return (ft_strncmp((char *)data, ref, ft_strlen(ref)));
}
