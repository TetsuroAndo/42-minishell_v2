/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:13:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 01:27:59 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

t_status    ms_setenv(const char *arg, t_shell *shell)
{
    t_list *lst;
    char key[PATH_MAX];
    char *entry;

    entry = ms_strdup(arg, shell);
    if (ms_partenvarg(key, arg) == E_ENV_KEY)
        return (E_ENV_KEY);
    lst = ft_list_find(shell->env_map, (void *)key, ms_envcmp);
    if (lst)
    {
        free(lst->data);
        lst->data = entry;
        return (E_NONE);
    }
    lst = xlstnew(entry, shell);
	ft_lstadd_back(&shell->env_map, lst);
    return (E_NONE);
}

t_status    ms_setenv_item(const char *key, const char *value, t_shell *shell)
{
    char entry[PATH_MAX];

    ft_strlcpy(entry, key, PATH_MAX);
    ft_strlcat(entry, "=", PATH_MAX);
    ft_strlcat(entry, value, PATH_MAX);
    return (ms_setenv(entry, shell));
}
