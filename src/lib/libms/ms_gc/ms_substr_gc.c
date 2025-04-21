/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_substr_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 03:13:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 12:57:06 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_substr_gcline(char const *s, unsigned int start, size_t len, t_shell *sh)
{
	return (ms_strndup_gcline(s + ft_strnlen(s, start), len, sh));
}


char	*ms_substr_r_gcline(char const *s, char delimiter, t_shell *sh)
{
	char	*pos;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcline("", sh));
	return (ms_strdup_gcline(pos + 1, sh));
}

char	*ms_substr_l_gcline(const char *s, char delimiter, t_shell *sh)
{
	char	*pos;
	size_t	len;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcline(s, sh));
	len = (size_t)(pos - s);
	return (ms_strndup_gcline(s, len, sh));
}
