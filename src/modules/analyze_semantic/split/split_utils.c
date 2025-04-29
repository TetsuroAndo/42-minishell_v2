/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:58:01 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/29 20:01:55 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

size_t	get_next_token(char **p, char **start)
{
	char	*pos;

	pos = *p;
	while (*pos && ft_isspace((unsigned char)*pos))
		pos++;
	if (!*pos)
		return (0);
	*start = pos;
	while (*pos && !ft_isspace((unsigned char)*pos))
		pos++;
	*p = pos;
	return (pos - *start);
}

void	free_split_result(char **result, t_shell *sh)
{
	size_t	i;

	if (!result)
		return ;
	i = 0;
	while (result[i])
	{
		ft_gc_free(sh->gcli, (void **)&result[i]);
		i++;
	}
	ft_gc_free(sh->gcli, (void **)&result);
}

size_t	count_aft_wc_tok(char *s)
{
	size_t	count;
	char	*p;

	count = 0;
	p = s;
	while (*p)
	{
		while (*p && isspace((unsigned char)*p))
			p++;
		if (!*p)
			break ;
		count++;
		while (*p && !ft_isspace((unsigned char)*p))
			p++;
	}
	return (count);
}
