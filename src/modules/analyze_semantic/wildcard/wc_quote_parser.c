/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_quote_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:43:38 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 19:51:05 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

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
	int	i;

	(void)str;
	i = start;
	while (i <= end)
	{
		map[i] = EX_IN;
		i++;
	}
}

static void	handle_unclosed_quote(char *map, int start, int len)
{
	int	i;

	i = start;
	while (i < len)
	{
		map[i] = EX_OUT;
		i++;
	}
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

static void	exi_roop(t_extract *res, char *str, char *map)
{
	int		i;
	int		r;
	int		in;
	char	q;

	i = 0;
	r = 0;
	in = 0;
	q = 0;
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
}

static t_extract	*extract_inner(char *str, char *map, t_shell *shell)
{
	t_extract	*res;
	int			len;

	res = xmalloc_gcline(sizeof(t_extract), shell);
	if (!res)
		return (NULL);
	len = strlen(str);
	res->str = xmalloc_gcline(len + 1, shell);
	res->map = xmalloc_gcline(len + 1, shell);
	exi_roop(res, str, map);
	return (res);
}

t_extract	*convert_ex(char *str, t_shell *shell)
{
	t_extract	*ex;
	char		*map;
	size_t		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	map = (char *)xmalloc_gcline(len + 1, shell);
	init_map(map, len);
	mark_quotes(str, map);
	ex = extract_inner(str, map, shell);
	ft_gc_free(shell->gcli, (void **)&map);
	return (ex);
}

char	*replace_with_unquoted(char *str, t_shell *shell)
{
	t_extract	*ex;
	char		*result;

	if (!str)
		return (ms_strdup("", shell));
	ex = convert_ex(str, shell);
	if (!ex)
		return (ms_strdup("", shell));
	result = ex->str;
	ft_gc_free(shell->gcli, (void **)&ex->map);
	ft_gc_free(shell->gcli, (void **)&ex);
	return (result);
}

void	free_exstract(t_extract *ex, t_shell *shell)
{
	ft_gc_free(shell->gcli, (void **)&ex->map);
	ft_gc_free(shell->gcli, (void **)&ex->str);
	ft_gc_free(shell->gcli, (void **)&ex);
}
