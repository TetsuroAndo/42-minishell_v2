/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xstrs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:21:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 00:42:58 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

char	**xsplit(char *str, char sep, t_shell *shell)
{
	char	**split;

	split = ft_split(str, sep);
	if (!split)
		shell_exit(shell, E_ALLOCATE);
	return (split);
}

char *xstrjoin_free(char const *s1, char const *s2, t_shell *shell)
{
	char *str;

	if (!s1 || !s2)
		return (NULL);
	str = ft_strjoin(s1, s2);
	if (s1)
		free((void *)s1);
	if (!str)
		shell_exit(shell, E_ALLOCATE);
	return (str);
}
