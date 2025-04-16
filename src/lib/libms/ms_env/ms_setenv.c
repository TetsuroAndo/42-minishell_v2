/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:13:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 08:46:27 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static char *ms_validate_value(const char *arg, t_shell *shell)
{
    char *value;

    value = ms_substr_r(arg, '=', shell);
    if (!ms_isactivevalue(value)) // $とその他の特殊記号が含まれている場合は$をエスケープ
        value = ms_escapevalue(value, shell);
    if (!value)
        return (ms_strdup("", shell));
    return (value);
}


t_status    ms_setenv(char *entry, t_shell *shell)
{
    t_list *lst;
    char key[PATH_MAX];
    char *value;
    
    if (ms_partenvarg(key, entry) != E_NONE)
        return (E_ENV_KEY);
    value = ms_validate_value(entry, shell);
    free(entry);
    entry = xstrjoin3(key, "=", value, shell);
    free(value);
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
    char *entry;
    
    entry = xstrjoin3(key, "=", value, shell);
    return (ms_setenv(entry, shell));
}
