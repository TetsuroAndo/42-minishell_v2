/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strjoin_gc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 20:09:45 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 20:11:45 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*ms_strjoin_gcli(char const *s1, char const *s2, t_shell *sh)
{
	char	*r;
	size_t	size;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	r = (char *)xmalloc_gcline(sizeof(char) * size, sh);
	if (!r)
		shell_exit(sh, E_SYSTEM);
	ft_strlcpy(r, s1, size);
	ft_strlcat(r, s2, size);
	return (r);
}

char	*ms_strjoin_gcli_f2(char const *s1, char const *s2, t_shell *sh)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = ms_strjoin_gcli(s1, s2, sh);
	if (s1)
		xfree((void **)&s1);
	if (s2)
		xfree((void **)&s2);
	return (str);
}
