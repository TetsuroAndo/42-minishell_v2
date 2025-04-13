/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 03:13:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/13 22:36:31 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char *ms_substr(char const *s, unsigned int start, size_t len, t_shell *shell)
{
    char *str;

    if (!s)
        return (NULL);
    if (start >= ft_strlen(s))
        return (ms_strdup("", shell));
    str = ft_substr(s, start, len);
    if(!str)
        shell_exit(shell, E_ALLOCATE);
    return (str);
}

char *ms_substr_r(char const *s, char delimiter, t_shell *shell)
{
    char *str;

    if (!s)
        return (NULL);
    str = ft_substr_r(s, delimiter);
    if(!str)
        shell_exit(shell, E_ALLOCATE);
    return (str);
}

char *ms_substr_l(const char *s, char delimiter, t_shell *shell)
{
    char *str;

    if (!s)
        return (NULL);
    str = ft_substr_l(s, delimiter);
    if(!str)
        shell_exit(shell, E_ALLOCATE);
    return (str);
}
