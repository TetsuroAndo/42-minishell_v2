/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:07:19 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 20:08:26 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

void	init_map(char *map, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		map[i] = EX_OUT;
		i++;
	}
	map[i] = '\0';
}

void	set_quote_map(char *str, char *map, int start, int end)
{
	int	i;

	(void)str;
	i = start;
	while (i <= end)
	{
		map[i] = EX_IN;
		i++;
	}
}

void	handle_unclosed_quote(char *map, int start, int len)
{
	int	i;

	i = start;
	while (i < len)
	{
		map[i] = EX_OUT;
		i++;
	}
}

void	mark_quotes(char *str, char *map)
{
	int		i;
	int		start;
	char	q;

	i = 0;
	start = -1;
	q = 0;
	while (str[i])
	{
		if (!q && (str[i] == '\'' || str[i] == '\"'))
		{
			q = str[i];
			start = i;
		}
		else if (q && str[i] == q)
		{
			set_quote_map(str, map, start, i);
			q = 0;
		}
		else if (q)
			map[i] = EX_IN;
		i++;
	}
	if (q)
		handle_unclosed_quote(map, start, i);
}

void	copy_char(t_extract *res, char c, char m, int *r)
{
	res->str[*r] = c;
	res->map[(*r)++] = m;
}
