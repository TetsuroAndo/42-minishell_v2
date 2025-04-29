/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:49 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 07:56:20 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	free_token(void *token)
{
	t_lexical_token	*t;

	if (!token)
		return ;
	t = (t_lexical_token *)token;
	if (t->value)
		xfree((void **)&t->value);
	xfree((void **)&t);
}
