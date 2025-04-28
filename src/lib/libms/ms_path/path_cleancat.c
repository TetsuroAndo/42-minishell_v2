/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_cleancat.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 03:07:58 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:08:12 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

size_t	ms_path_cleancat(char *dst, const char *s, size_t siz)
{
	size_t	didx;
	size_t	sidx;
	char	prev;

	sidx = 0;
	if (!dst || siz == 0)
		return (0);
	didx = ft_strlen(dst);
	prev = 0;
	if (s)
	{
		while (s[sidx] && didx < siz - 1)
		{
			if (s[sidx] == '/' && prev == '/')
			{
				sidx++;
				continue ;
			}
			dst[didx++] = s[sidx];
			prev = s[sidx++];
		}
	}
	dst[didx] = '\0';
	return (didx);
}
