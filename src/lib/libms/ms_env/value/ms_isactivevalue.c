/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_isactivevalue.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 07:44:31 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 19:13:06 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	ms_isactivevalue(const char *value)
{
	size_t	i;

	if (!value)
		return (1);
	i = 0;
	while (value[i])
	{
		if ((unsigned char)value[i] < 32 || value[i] == 127)
			return (0);
		if (ft_strchr("\"'`|&;<>()[]{}\\*?#~", value[i]))
			return (0);
		i++;
	}
	return (1);
}
