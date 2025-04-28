/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:23:55 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 15:33:37 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	skip_quoted_word(const char *line, size_t *pos, t_shell *shell)
{
	char	quote;

	quote = line[*pos];
	(*pos)++;
	while (line[*pos] && line[*pos] != quote)
		(*pos)++;
	if (!line[*pos])
	{
		shell->status = E_SYNTAX;
		return (1);
	}
	(*pos)++;
	return (0);
}
