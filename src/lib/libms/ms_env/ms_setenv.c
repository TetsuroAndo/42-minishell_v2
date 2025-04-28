/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:13:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 19:33:29 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static char	*ms_validate_value(const char *arg, t_shell *sh)
{
	char	*value;

	value = ms_substr_r(arg, '=', sh);
	if (!ms_isactivevalue(value))
		value = ms_escapevalue(value, sh);
	if (!value)
		return (ms_strdup("", sh));
	return (value);
}

static char	*ms_handle_append_env(const char *key, const char *arg,
		size_t eq_pos, t_shell *sh)
{
	char	*exist_value;
	char	*new_value;
	char	*joined;
	char	*new_entry;

	(void)eq_pos;
	exist_value = ms_getenv(key, sh);
	new_value = ms_validate_value(arg, sh);
	joined = xstrjoin_free2(exist_value, new_value, sh);
	new_entry = xstrjoin3(key, "=", joined, sh);
	xfree((void **)&joined);
	return (new_entry);
}

static char	*ms_handle_normal_env(const char *key, const char *arg,
		size_t eq_pos, t_shell *sh)
{
	t_list	*lst;
	char	*value;
	char	*new_entry;

	if (eq_pos == 0)
	{
		lst = ft_list_find(sh->env_map, (void *)key, ms_envcmp);
		if (lst && ft_strchr(lst->data, '='))
			return (ms_strdup(lst->data, sh));
		return (ms_strdup(key, sh));
	}
	else if (arg[eq_pos] == '\0')
	{
		value = ms_strdup("", sh);
	}
	else
	{
		value = ms_validate_value(arg, sh);
	}
	new_entry = xstrjoin3(key, "=", value, sh);
	xfree((void **)&value);
	return (new_entry);
}

t_status	ms_setenv(char *entry, t_shell *sh)
{
	t_list	*lst;
	char	key[PATH_MAX];
	char	*new_entry;
	int		is_append;
	size_t	eq_pos;

	is_append = 0;
	eq_pos = 0;
	if (ms_partenvarg(key, entry, &is_append, &eq_pos) != E_NONE)
		return (xfree((void **)&entry), E_ENV_KEY);
	if (is_append)
		new_entry = ms_handle_append_env(key, entry, eq_pos, sh);
	else
		new_entry = ms_handle_normal_env(key, entry, eq_pos, sh);
	xfree((void **)&entry);
	lst = ft_list_find(sh->env_map, (void *)key, ms_envcmp);
	if (lst)
	{
		xfree((void **)&lst->data);
		lst->data = new_entry;
		return (E_NONE);
	}
	lst = xlstnew(new_entry, sh);
	ft_lstadd_back(&sh->env_map, lst);
	return (E_NONE);
}

t_status	ms_setenv_item(const char *key, const char *value, t_shell *sh)
{
	char	*entry;

	entry = xstrjoin3(key, "=", value, sh);
	return (ms_setenv(entry, sh));
}
