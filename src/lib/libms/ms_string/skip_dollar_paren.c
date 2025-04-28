/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_dollar_paren.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:48:51 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:10:04 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	skip_dollar_paren(const char *line, size_t *pos)
{
	int	depth;

	if (!(line[*pos] == '$' && line[*pos + 1] == '('))
		return ;
	(*pos) += 2;
	depth = 1;
	while (line[*pos] && depth)
	{
		if (line[*pos] == '$' && line[*pos + 1] == '(')
			depth++;
		else if (line[*pos] == ')')
			depth--;
		(*pos)++;
	}
}
