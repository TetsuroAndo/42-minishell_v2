/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:18:48 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 01:08:19 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_status	ms_unset(const char *key, t_shell *shell)
{
    ft_list_remove_if(&shell->env_map, (void *)key, ms_envcmp, free);
    return (E_NONE);
}
