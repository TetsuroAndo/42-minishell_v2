/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_cleanstrlcpy.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 06:53:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 07:05:26 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

size_t	ms_path_cleancpy(char *dst, const char *s, size_t siz)
{
	size_t	sidx;
	size_t	didx;
	char	prev;

	sidx = 0;
	didx = 0;
	prev = 0;
	if (siz == 0 || !s)
		return (0);
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
	dst[didx] = '\0';
	return (didx);
}
