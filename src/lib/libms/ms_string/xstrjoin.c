/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xstrjoin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:38:37 by teando            #+#    #+#             */
/*   Updated: 2025/04/16 10:44:49 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char    *xstrjoin(char const *s1, char const *s2, t_shell *shell)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = ft_strjoin(s1, s2);
	if (!str)
		shell_exit(shell, E_ALLOCATE);
	return (str);
}

char	*xstrjoin_free(char const *s1, char const *s2, t_shell *shell)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = xstrjoin(s1, s2, shell);
	if (s1)
		free((void *)s1);
	return (str);
}

char	*xstrjoin_free2(char const *s1, char const *s2, t_shell *shell)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = xstrjoin(s1, s2, shell);
	if (s1)
		free((void *)s1);
	if (s2)
		free((void *)s2);
	return (str);
}
