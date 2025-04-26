/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:57:56 by teando            #+#    #+#             */
/*   Updated: 2025/04/26 20:28:21 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_sem.h"

t_quote_state	is_quote_type(int c)
{
	if (c == '"')
		return (QS_DOUBLE);
	if (c == '\'')
		return (QS_SINGLE);
	if (c == '`')
		return (QS_BACK);
	return (QS_NONE);
}

int	check_qs(int c, t_expenv *sem)
{
	if (sem->quote_state == QS_NONE)
		sem->quote_state = is_quote_type(c);
	else if (sem->quote_state == is_quote_type(c))
		sem->quote_state = QS_NONE;
	return (c);
}
