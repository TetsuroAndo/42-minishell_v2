/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:15:13 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/10 14:15:53 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

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
	update_dp_row(str, prev, curr, ex);
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
