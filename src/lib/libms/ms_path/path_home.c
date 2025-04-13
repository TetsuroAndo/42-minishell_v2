/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_home.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:18:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 01:37:14 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	path_home(char path[], const char *src, int mode, t_shell *shell)
{
    char *home;

    home = ms_getenv("HOME", shell);
    ft_strlcpy(path, home, PATH_MAX);
    ft_strlcat(path, src + 1, PATH_MAX);
    free(home);
    return (access(path, mode));
}

