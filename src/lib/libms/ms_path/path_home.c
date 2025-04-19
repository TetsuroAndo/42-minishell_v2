/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_home.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:18:44 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 07:56:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	path_home(char path[], const char *src, int mode, t_shell *shell)
{
    char *home;

    home = ms_getenv("HOME", shell);
    ft_strlcpy(path, home, PATH_MAX + 1);
    ft_strlcat(path, src + 1, PATH_MAX + 1);
    xfree((void **)&home);
    return (access(path, mode));
}
