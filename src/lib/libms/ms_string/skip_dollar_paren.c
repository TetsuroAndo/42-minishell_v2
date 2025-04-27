/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_dollar_paren.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:48:51 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 22:10:53 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	skip_dollar_paren(const char *line, size_t *pos)
{
	int depth;

	if (!(line[*pos] == '$' && line[*pos + 1] == '('))
		return ;
	/* "$(" を読み飛ばしてから走査開始 */
	(*pos) += 2;
	depth = 1;
	while (line[*pos] && depth)
	{
		if (line[*pos] == '(')
			depth++;
		else if (line[*pos] == ')')
			depth--;
		(*pos)++;
	}
}
