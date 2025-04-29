/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:09:49 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 20:25:36 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

int	is_invalid_input(const char *p, const char *str, t_extract *ex)
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

int	*init_dp_row(int n, t_shell *shell)
{
	int	*row;
	int	i;

	row = xmalloc_gcline(sizeof(int) * (n + 1), shell);
	i = 0;
	while (i <= n)
		row[i++] = 0;
	return (row);
}

void	update_first_cell(char p_char, int *prev, int *curr, char map_char)
{
	if (p_char == '*' && map_char == EX_OUT)
		curr[0] = prev[0];
	else
		curr[0] = 0;
}

void	process_row_cells(char p_char, const char *s, int *prev, int *curr)
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

void	update_dp_row(const char *s, int *prev, int *curr, t_extract *ex)
{
	int		m;
	int		i;
	char	p_char;

	m = ft_strlen(ex->str);
	i = 1;
	while (i <= m)
	{
		if (ex->map[i - 1] == EX_OUT && ex->str[i - 1] == '*')
			p_char = '*';
		else
			p_char = ex->str[i - 1];
		update_first_cell(p_char, prev, curr, ex->map[i - 1]);
		process_row_cells(p_char, s, prev, curr);
		ft_swapint(&prev, &curr);
		i++;
	}
}
