/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:08:37 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/10 11:19:22 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	wildcard_match(const char *p, const char *str, t_shell *shell)
{
	int			n;
	int			*prev;
	int			*curr;
	int			result;
	t_extract	*ex;

	if (!p || !str)
		return (0);
	n = ft_strlen(str);
	ex = convert_ex((char *)p, shell);
	if (is_invalid_input(p, str, ex))
		return (free_exstract(ex, shell), 0);
	prev = init_dp_row(n, shell);
	curr = init_dp_row(n, shell);
	prev[0] = 1;
	update_dp_row(str, prev, curr, ex);
	if (ft_strlen(ex->str) % 2 == 0)
		result = prev[n];
	else
		result = curr[n];
	ft_gc_free(shell->gcli, (void **)&prev);
	ft_gc_free(shell->gcli, (void **)&curr);
	if (ex)
		free_exstract(ex, shell);
	return (result);
}

static char	*append_match(char *buf, const char *name, int flag, t_shell *sh)
{
	char	*new_buf;

	if (!buf)
	{
		if (flag)
			return (ms_strjoin_gcli(name, "/", sh));
		else
			return (ms_strdup_gcli(name, sh));
	}
	new_buf = ms_strjoin3_gcli(buf, " ", name, sh);
	if (flag)
		new_buf = ms_strjoin_gcli(new_buf, "/", sh);
	ft_gc_free(sh->gcli, (void **)&buf);
	return (new_buf);
}

static char	*collect_matches(DIR *dir, const char *pattern, t_shell *sh)
{
	struct dirent	*entry;
	char			*buf;

	if (!dir || !pattern)
		return (NULL);
	buf = NULL;
	while (42)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_name[0] != '.')
		{
			if (wildcard_match(pattern, entry->d_name, sh))
				buf = append_match(buf, entry->d_name, 0, sh);
		}
		else if ((((ft_strncmp(entry->d_name, ".", 1) == 0
						|| ft_strncmp(entry->d_name, "..", 2) == 0))
				&& pattern[0] == '.') && wildcard_match(pattern, entry->d_name,
				sh))
		{
			buf = append_match(buf, entry->d_name, 0, sh);
		}
	}
	return (buf);
}

static char	*collect_matche_dir(DIR *dir, const char *pattern, t_shell *sh)
{
	struct dirent	*entry;
	char			*buf;

	if (!dir || !pattern)
		return (NULL);
	buf = NULL;
	while (42)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_type != DT_DIR)
			continue ;
		if (entry->d_name[0] != '.')
		{
			if (wildcard_match(pattern, entry->d_name, sh))
				buf = append_match(buf, entry->d_name, 1, sh);
		}
		else if ((((ft_strncmp(entry->d_name, ".", 1) == 0
						|| ft_strncmp(entry->d_name, "..", 2) == 0))
				&& pattern[0] == '.') && wildcard_match(pattern, entry->d_name,
				sh))
			buf = append_match(buf, entry->d_name, 1, sh);
	}
	return (buf);
}

static char	*handle_wildcard(char *in, t_shell *sh)
{
	DIR		*dir;
	char	*buf;
	char	*tmp;

	if (!in)
		return (NULL);
	tmp = ms_strdup_gcli(in, sh);
	ft_gc_free(sh->gcli, (void **)&in);
	dir = opendir(sh->cwd);
	if (!dir)
		return (tmp);
	if (tmp[ft_strlen(tmp) - 1] == '/')
	{
		tmp[ft_strlen(tmp) - 1] = '\0';
		buf = collect_matche_dir(dir, tmp, sh);
	}
	else
		buf = collect_matches(dir, tmp, sh);
	closedir(dir);
	if (buf)
		return (buf);
	return (tmp);
}

int	proc_wildcard(t_list **lst, int index, t_shell *sh)
{
	t_lexical_token	*tok;
	char			*tmp;

	(void)index;
	tok = (*lst)->data;
	tmp = ms_strdup_gcli(tok->value, sh);
	if (ft_strchr(tok->value, '*'))
		tmp = handle_wildcard(tmp, sh);
	ft_gc_untrack(sh->gcli, tmp);
	xfree((void **)&tok->value);
	tok->value = tmp;
	return (0);
}
