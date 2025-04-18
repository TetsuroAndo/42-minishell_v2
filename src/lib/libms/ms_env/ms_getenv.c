/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:24:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 21:10:28 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char *ms_getenv(const char *key, t_shell *shell)
{
    t_list *lst;
    
    if(key[0] && key[1] == '\0' && shell->env_spc[(unsigned char)*key])
        return (shell->env_spc[(unsigned char)*key]);
    lst = ft_list_find(shell->env_map, (void *)key, ms_envcmp);
    if (!lst)
        return (ms_strdup("", shell));
    return (ms_substr_r(lst->data, '=', shell));
}
