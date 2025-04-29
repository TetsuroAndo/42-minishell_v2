/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_quote_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:43:38 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 20:07:06 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

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
