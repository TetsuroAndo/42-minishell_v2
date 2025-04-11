/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 03:13:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 16:52:45 by teando           ###   ########.fr       */
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
