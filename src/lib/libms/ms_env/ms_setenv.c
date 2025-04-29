/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 22:13:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 19:43:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static char	*ms_validate_value(const char *arg, t_shell *sh)
{
	char	*val;
	char	*esca_val;

	val = ms_substr_r_gcli(arg, '=', sh);
	if (!val)
		return (ms_strdup("", sh));
	if (!ms_isactivevalue(val))
	{
		esca_val = ms_escapevalue(val, sh);
		xfree_gc((void **)&val, sh);
		return (esca_val);
	}
	return (val);
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
	if (!new_value)
	{
		return (exist_value);
	}
	joined = xstrjoin_free2(exist_value, new_value, sh);
	new_entry = xstrjoin3(key, "=", joined, sh);
	xfree_gc((void **)&joined, sh);
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
	xfree_gc((void **)&value, sh);
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
		return (xfree_gc((void **)&entry, sh), E_ENV_KEY);
	if (is_append)
		new_entry = ms_handle_append_env(key, entry, eq_pos, sh);
	else
		new_entry = ms_handle_normal_env(key, entry, eq_pos, sh);
	xfree_gc((void **)&entry, sh);
	lst = ft_list_find(sh->env_map, (void *)key, ms_envcmp);
	if (lst)
	{
		xfree_gc((void **)&lst->data, sh);
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
