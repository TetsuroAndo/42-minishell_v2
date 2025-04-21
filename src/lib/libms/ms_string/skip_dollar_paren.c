/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_dollar_paren.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:48:51 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 07:54:19 by teando           ###   ########.fr       */
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
		/* バックスラッシュでエスケープされた 1 文字を丸ごとスキップ */
		if (line[*pos] == '\\' && line[*pos + 1])
		{
			(*pos) += 2;
			continue ;
		}
		if (line[*pos] == '(')
			depth++;
		else if (line[*pos] == ')')
			depth--;
		(*pos)++;
	}
}
