/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_valid_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:57:33 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 15:50:35 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

char	*trim_valid_quotes(const char *s, t_shell *sh)
{
	char	*out;
	char	*dst;
	char	quote;
	size_t	i;

	i = 0;
	quote = 0;
	out = xmalloc(ft_strlen(s) + 1, sh);
	dst = out;
	while (s[i])
	{
		if (!quote && ft_strchr("'\"`", s[i]))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else
			*dst++ = s[i];
		++i;
	}
	if (quote)
		return (xfree((void **)&out), ms_strdup(s, sh));
	return (out);
}
