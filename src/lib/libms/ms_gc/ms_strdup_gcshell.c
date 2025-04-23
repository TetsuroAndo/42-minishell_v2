/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strdup_gcshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:46:13 by teando            #+#    #+#             */
/*   Updated: 2025/04/23 16:16:11 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_strndup_gcshell(const char *s, size_t n, t_shell *sh)
{
	char	*r;
	size_t	len;

	if (!s)
		return (NULL);
	r = (char *)xmalloc_gcshell((n + 1) * sizeof(char), sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	len = ft_strnlen(s, n);
	r[len] = '\0';
	return (ft_memcpy(r, s, len));
}

char	*ms_strdup_gcshell(const char *s, t_shell *sh)
{
	return (ms_strndup_gcshell(s, ft_strlen(s), sh));
}
