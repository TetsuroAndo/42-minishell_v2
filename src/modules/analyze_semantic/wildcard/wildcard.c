/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:08:37 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/26 22:10:39 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

static int	is_invalid_input(const char *p, const char *str, t_extract *ex)
{
	int		i;
	int		non_star;
	size_t	slen;

	if (!p || !str || !ex)
		return (1);
	slen = ft_strlen(str);
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

static int	*init_dp_row(int n, t_shell *shell)
{
	int	*row;
	int	i;

	row = xmalloc_gcline(sizeof(int) * (n + 1), shell);
	i = 0;
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

static void	update_dp_row(const char *p, const char *s, int *prev, int *curr,
		t_extract *ex)
{
	int	m;
	int	i;

	(void)p;
	m = ft_strlen(ex->str);
	i = 1;
	while (i <= m)
	{
		update_first_cell(ex->str[i - 1], prev, curr, ex->map[i - 1]);
		process_row_cells(ex->str[i - 1], s, prev, curr, ex->map[i - 1]);
		ft_swapint(&prev, &curr);
		i++;
	}
}

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
	update_dp_row(ex->str, str, prev, curr, ex);
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

static char	*append_match(char *buf, const char *name, t_shell *sh)
{
	char	*new_buf;

	if (!buf)
		return (ms_strdup_gcli(name, sh));
	new_buf = ms_strjoin3_gcli(buf, " ", name,sh);
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
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] != '.')
		{
			if (wildcard_match(pattern, entry->d_name, sh))
				buf = append_match(buf, entry->d_name, sh);
		}
		else if ((ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name,
					"..") == 0) && pattern[0] == '.' && wildcard_match(pattern,
				entry->d_name, sh))
		{
			buf = append_match(buf, entry->d_name, sh);
		}
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
	if (strchr(tok->value, '*'))
		tmp = handle_wildcard(tmp, sh);
	ft_gc_untrack(sh->gcli, tmp);
	xfree((void **)&tok->value);
	tok->value = tmp;
	return (0);
}
