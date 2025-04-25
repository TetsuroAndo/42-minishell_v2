/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:55:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 10:23:21 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* Wildcard                                                                   */
/* -------------------------------------------------------------------------- */

static int	is_invalid_input(const char *p, const char *str, t_extract *ex)
{
	int		i;
	int		non_star;
	size_t	slen;

	if (!p || !str || !ex)
		return (1);
	slen = strlen(str);
	non_star = 0;
	i = 0;
	while (ex->str[i])
	{
		if (ex->str[i] == '*' && ex->map[i] == EX_OUT)
			;
		else
			non_star++;
		i++;
	}
	return (non_star > (int)slen);
}

static int	*init_dp_row(int n)
{
	int	*row;
	int	i;

	row = malloc(sizeof(int) * (n + 1));
	i = 0;
	if (!row)
		return (NULL);
	while (i <= n)
		row[i++] = 0;
	return (row);
}

static void	update_first_cell(char p_char, int *prev, int *curr, char map_char)
{
	if (p_char == '*' && map_char == EX_OUT)
		curr[0] = prev[0];
	else
		curr[0] = 0;
}

static void	process_row_cells(char p_char, const char *s, int *prev, int *curr,
		char map_char)
{
	int	j;
	int	n;

	n = strlen(s);
	j = 1;
	while (j <= n)
	{
		if (p_char == '*' && map_char == EX_OUT)
			curr[j] = prev[j] || curr[j - 1];
		else
		{
			if (p_char == s[j - 1])
				curr[j] = prev[j - 1];
			else
				curr[j] = 0;
		}
		j++;
	}
}

/* ワイルドカード DP 更新 */
static void	update_dp_row(const char *p, const char *s, int *prev, int *curr,
		t_extract *ex)
{
	int	m;
	int	i;

	(void)p;
	m = ft_strlen(ex->str); // Changed from strlen(p) to strlen(ex->str)
	i = 1;
	while (i <= m)
	{
		update_first_cell(ex->str[i - 1], prev, curr, ex->map[i - 1]);
		// Changed p[i-1] to ex->str[i-1]
		process_row_cells(ex->str[i - 1], s, prev, curr, ex->map[i - 1]);
		// Changed p[i-1] to ex->str[i-1]
		ft_swapint(&prev, &curr);
		i++;
	}
}

int	wildcard_match(const char *p, const char *str, t_shell *shell)
{
	int			m;
	int			n;
	int			*prev;
	int			*curr;
	int			result;
	t_extract	*ex;

	n = ft_strlen(str);
	ex = convert_ex((char *)p, shell);
	if (is_invalid_input(p, str, ex))
	{
		if (ex)
			return (xfree((void **)&ex->str), xfree((void **)&ex->map),
				xfree((void **)&ex), 0);
		return (0);
	}
	m = ft_strlen(ex->str);
	prev = init_dp_row(n);
	curr = init_dp_row(n);
	if (!prev || !curr)
	{
		if (ex)
			return (xfree((void **)&ex->str), xfree((void **)&ex->map),
				xfree((void **)&ex), 0);
		return (0);
	}
	prev[0] = 1;
	update_dp_row(ex->str, str, prev, curr, ex);
	if (m % 2 == 0)
		result = prev[n];
	else
		result = curr[n];
	xfree((void **)&prev), xfree((void **)&curr);
	if (ex)
		xfree((void **)&ex->str), xfree((void **)&ex->map), xfree((void **)&ex);
	return (result);
}

static char	*append_match(char *buf, const char *name, t_shell *sh)
{
	char	*new_buf;

	if (!buf)
		return (ms_strdup(name, sh));
	new_buf = xstrjoin3(buf, " ", name, sh);
	xfree((void **)&buf);
	return (new_buf);
}

static char	*collect_matches(DIR *dir, const char *pattern, t_shell *sh)
{
	struct dirent	*entry;
	char			*buf;

	buf = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".", 1) != 0 && ft_strncmp(entry->d_name,
				"..", 2) != 0 && wildcard_match(pattern, entry->d_name, sh))
		{
			buf = append_match(buf, entry->d_name, sh);
			if (!buf)
				break ;
		}
		else if (((ft_strncmp(entry->d_name, ".", 1) == 0
					&& ft_strncmp(entry->d_name, "..", 1) == 0)
				|| (ft_strncmp(entry->d_name, "..", 2) == 0
					&& ft_strncmp(entry->d_name, "..", 2) == 0))
			&& pattern[0] == '.' && wildcard_match(pattern, entry->d_name, sh))
		{
			buf = append_match(buf, entry->d_name, sh);
			if (!buf)
				break ;
		}
		entry = readdir(dir);
	}
	return (buf);
}

static char	*process_split_wildcard(char **split, t_shell *sh)
{
	char	*buf;
	char	*tmp;
	char	*joined;
	int		i;

	buf = NULL;
	i = 0;
	while (split[i])
	{
		tmp = handle_wildcard(split[i], sh);
		if (!tmp)
			continue ;
		if (!buf)
			buf = ms_strdup(tmp, sh);
		else
		{
			joined = xstrjoin3(buf, " ", tmp, sh);
			xfree((void **)&buf);
			buf = joined;
		}
		i++;
	}
	return (buf);
}

static char	*process_directory_wildcard(char *in, t_shell *sh)
{
	DIR		*dir;
	char	*buf;

	dir = opendir(sh->cwd);
	if (!dir)
		return (in);
	buf = collect_matches(dir, in, sh);
	closedir(dir);
	if (buf)
	{
		xfree((void **)&in);
		return (buf);
	}
	return (in);
}

char	*handle_wildcard(char *in, t_shell *sh)
{
	char	**split;
	char	*buf;

	if (!in || !sh)
		return (NULL);
	if (ft_strchr(in, ' '))
	{
		split = xsplit(ms_strdup(in, sh), ' ', sh);
		if (!split)
			return (NULL);
		buf = process_split_wildcard(split, sh);
		ft_strs_clear(split);
		return (buf);
	}
	return (process_directory_wildcard(in, sh));
}
