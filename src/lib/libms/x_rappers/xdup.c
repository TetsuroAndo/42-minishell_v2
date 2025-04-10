/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xdup.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:18:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:42:29 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

char	*xdup(char *str, t_shell *shell)
{
	char	*dup;

	dup = (char *)xmalloc(sizeof(char) * (ft_strlen(str) + 1), shell);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, str, ft_strlen(str));
	dup[ft_strlen(str)] = '\0';
	return (dup);
}

char	*xdup2(char *str, t_shell *shell)
{
	char	*dup;

	dup = (char *)xmalloc(sizeof(char) * (ft_strlen(str) + 1), shell);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, str, ft_strlen(str));
	dup[ft_strlen(str)] = '\0';
	return (dup);
}
