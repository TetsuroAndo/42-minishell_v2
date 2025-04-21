/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strdup_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:51:16 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 20:03:50 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_strndup_gcshell(const char *s, size_t n, t_shell *sh)
{
	char	*r;
	size_t	len;

	r = (char *)xmalloc_gcshell((n + 1) * sizeof(char), sh);
	len = ft_strnlen(s, n);
	if (!r)
		return (NULL);
	r[len] = '\0';
	return (ft_memcpy(r, s, len));
}

char	*ms_strdup_gcshell(const char *s, t_shell *sh)
{
	return (ms_strndup_gcshell(s, ft_strlen(s), sh));
}
