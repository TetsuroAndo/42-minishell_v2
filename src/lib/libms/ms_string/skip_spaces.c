/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:49:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:11:08 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	skip_spaces(const char *line, size_t *pos)
{
	while (line[*pos] && ft_isspace(line[*pos]))
		(*pos)++;
}
