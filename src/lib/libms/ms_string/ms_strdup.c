/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 03:13:23 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 13:36:49 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char *ms_strndup(const char *s, size_t n, t_shell *shell)
{
    char *str;

    if (!s || !n)
        return (NULL);
    str = ft_strndup(s, n);
    if (!str)
        shell_exit(shell, E_ALLOCATE);
    return (str);
}

char *ms_strdup(const char *s, t_shell *shell)
{
    return (ms_strndup(s, ft_strlen(s), shell));
}
