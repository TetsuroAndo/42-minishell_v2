/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_quote_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:43:38 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/19 00:30:48 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/* -------------------------------------------------------------------------- */
/* Process quote                                                              */
/* -------------------------------------------------------------------------- */

static void	init_map(char *map, size_t len)
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

static void	set_quote_map(char *str, char *map, int start, int end)
{
	for (int i = start; i <= end; i++)
		map[i] = EX_IN;
}

static void	handle_unclosed_quote(char *map, int start, int len)
{
	for (int i = start; i < len; i++)
		map[i] = EX_OUT;
}

static void	mark_quotes(char *str, char *map)
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

static void	copy_char(t_extract *res, char c, char m, int *r)
{
	res->str[*r] = c;
	res->map[(*r)++] = m;
}

static t_extract	*extract_inner(char *str, char *map, t_shell *shell)
{
	t_extract	*res;
	int			i;
	int			r;
	int			in;
	int			len;
	char		q;

	res = xmalloc(sizeof(t_extract), shell);
	i = 0;
	r = 0;
	in = 0;
	len = strlen(str);
	res->str = malloc(len + 1);
	res->map = malloc(len + 1);
	if (!res->str || !res->map)
		return (free(res->str), free(res->map), free(res), NULL);
	while (str[i])
	{
		if (!in && (str[i] == '\'' || str[i] == '\"'))
		{
			q = str[i];
			in = 1;
		}
		else if (in && str[i] == q)
			in = 0;
		else
			copy_char(res, str[i], map[i], &r);
		i++;
	}
	res->str[r] = '\0';
	res->map[r] = '\0';
	return (res);
}
// ...existing code...

t_extract	*convert_ex(char *str, t_shell *shell)
{
	t_extract	*ex;
	char		*map;
	size_t		len;

	len = ft_strlen(str);
	map = (char *)xmalloc(len + 1, shell);
	init_map(map, len);
	mark_quotes(str, map);
	ex = extract_inner(str, map, shell);
	free(map);
	return (ex);
}

char	*replace_with_unquoted(char *str, t_shell *shell)
{
	t_extract *ex;
	char *result;

	if (!str)
		return (NULL);
	ex = convert_ex(str, shell);
	if (!ex)
		return (NULL);
	result = ex->str;
	free(ex->map);
	free(ex);
	return (result);
}
