/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_substr_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 03:13:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 20:04:36 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_substr_gcshell(char const *s, unsigned int start, size_t len, t_shell *sh)
{
	return (ms_strndup_gcshell(s + ft_strnlen(s, start), len, sh));
}


char	*ms_substr_r_gcshell(char const *s, char delimiter, t_shell *sh)
{
	char	*pos;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcshell("", sh));
	return (ms_strdup_gcshell(pos + 1, sh));
}

char	*ms_substr_l_gcshell(const char *s, char delimiter, t_shell *sh)
{
	char	*pos;
	size_t	len;

	if (!s)
		return (NULL);
	pos = ft_strchr(s, delimiter);
	if (!pos)
		return (ms_strdup_gcshell(s, sh));
	len = (size_t)(pos - s);
	return (ms_strndup_gcshell(s, len, sh));
}
