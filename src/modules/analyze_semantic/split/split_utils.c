/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:58:01 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/30 12:48:35 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

/**
 * @brief Get the start position and length of the next token
 *
 * @param p Starting position for search
 * @param start Pointer to store token start position (for result)
 * @return size_t Token length, 0 if no token found
 */
size_t	get_next_token(char **p, char **start)
{
	char	*pos;
	char	quote;

	pos = *p;
	while (*pos && ft_isspace((unsigned char)*pos))
		pos++;
	if (!*pos)
		return (0);
	*start = pos;
	quote = 0;
	while (*pos && (quote || !ft_isspace((unsigned char)*pos)))
	{
		if (quote == 0 && (*pos == '\'' || *pos == '"'))
			quote = *pos;
		else if (quote && *pos == quote)
			quote = 0;
		pos++;
	}
	*p = pos;
	return (pos - *start);
}

/**
 * @brief Free the string array allocated by split_with_quote
 *
 * @param result String array to be freed
 * @param sh Shell information
 */
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

/**
 * @brief Count the number of tokens after word count considering quotes
 *
 * @param s Input string to count tokens from
 * @return size_t Number of tokens
 */
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
