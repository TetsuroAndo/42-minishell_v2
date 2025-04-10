/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xitoa.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:52:07 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:53:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char *xitoa(int n, t_shell *shell)
{
    char *ans;

    ans = ft_itoa(n);
    if (!ans)
        system_exit(shell, 1);
    return (ans);
}
