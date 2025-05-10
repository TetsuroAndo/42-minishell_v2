/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 23:38:19 by teando            #+#    #+#             */
/*   Updated: 2025/05/11 01:01:12 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

char	*heredoc_env(char *tx, t_shell *sh)
{
	t_expenv	s;
	size_t		i;

	s.buf = ms_strdup_gcli("", sh);
	s.quote_state = QS_NONE;
	while (*tx)
	{
		i = 0;
		while (tx[i] && ((tx[i] == '$' && tx[i + 1] == '(') || tx[i] != '$'))
			++i;
		s.buf = ms_strjoin_gcli(s.buf, ms_substr_gcli(tx, 0, i, sh), sh);
		tx += i;
		if (*tx == '$' && tx[1] && !ft_isspace(tx[1]))
			tx += extract_varname(&s.buf, tx + 1, sh) + 1;
		else if (*tx == '$')
		{
			s.buf = ms_strjoin_gcli(s.buf, "$", sh);
			++tx;
		}
	}
	return (s.buf);
}
