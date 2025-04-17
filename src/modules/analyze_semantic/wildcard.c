/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:55:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/17 23:06:02 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"
#include <stdlib.h>

typedef struct s_pat_elem {
    char	*str;
	int		*map;
} t_pat_elem;

t_pat_elem	*convert_str2pat(char *p, t_shell *shell)
{
	t_pat_elem	*pat;
	size_t	i;

	pat->str = ms_strdup(p, shell); //
	pat->map = (void *)xmalloc((ft_strlen(p) + 1) * sizeof(char),shell); //クォート
	while (p)
	{

	}

	return pat;
}

static int	is_invalid_input(const char *p, const char *s)
{
	int	i;
	int	non_star;

	i = 0;
	non_star = 0;
	if (!p || !s)
		return (1);
	while (p[i])
		if (p[i++] != '*')
			non_star++;
	return (non_star > (int)strlen(s));
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

static void	swap_rows(int **a, int **b)
{
	int	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	update_first_cell(char p_char, int *prev, int *curr)
{
	if (p_char == '*')
		curr[0] = prev[0];
	else
		curr[0] = 0;
}

static void	process_row_cells(char p_char, const char *s, int *prev, int *curr)
{
	int	j;
	int	n;

	n = strlen(s);
	j = 1;
	while (j <= n)
	{
		if (p_char == '*')
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
static void	update_dp_row(const char *p, const char *s, int *prev, int *curr)
{
	int	m;
	int	i;

	m = strlen(p);
	i = 1;
	while (i <= m)
	{
		update_first_cell(p[i - 1], prev, curr);
		process_row_cells(p[i - 1], s, prev, curr);
		swap_rows(&prev, &curr);
		i++;
	}
}

int	wildcard_match(const char *p, const char *s)
{
	int	m;
	int	n;
	int	*prev;
	int	*curr;
	int	result;

	m = ft_strlen(p);
	n = ft_strlen(s);
	if (is_invalid_input(p, s))
		return (0);
	prev = init_dp_row(n);
	curr = init_dp_row(n);
	if (!prev || !curr)
		return (0);
	prev[0] = 1;
	update_dp_row(p, s, prev, curr);
	if (m % 2 == 0)
		result = prev[n];
	else
		result = curr[n];
	free(prev);
	free(curr);
	return (result);
}

static char	*append_match(char *buf, const char *name, t_shell *sh)
{
	char	*new_buf;

	if (!buf)
		return (ms_strdup(name, sh));
	new_buf = ft_strjoin3(buf, " ", name);
	free(buf);
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
		if (ft_strncmp(entry->d_name, ".", 2) != 0 && ft_strncmp(entry->d_name,
				"..", 3) != 0 && wildcard_match(pattern, entry->d_name))
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
	for (i = 0; split[i]; i++)
	{
		tmp = handle_wildcard(split[i], sh);
		if (!tmp)
			continue ;
		if (!buf)
			buf = ms_strdup(tmp, sh);
		else
		{
			joined = ft_strjoin3(buf, " ", tmp);
			free(buf);
			buf = joined;
		}
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
		return (buf);
	else
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
		split = xsplit(in, ' ', sh);
		if (!split)
			return (NULL);
		buf = process_split_wildcard(split, sh);
		ft_strs_clear(split);
		return (buf);
	}
	return (process_directory_wildcard(in, sh));
}
