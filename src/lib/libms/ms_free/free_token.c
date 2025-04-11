/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:20:49 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 21:03:54 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"

/**
 * @brief トークンの解放
 *
 * @param token 解放するトークンへのポインタ
 */
void	free_token(void *token)
{
	t_lexical_token	*t;

	if (!token)
		return ;
	t = (t_lexical_token *)token;
	if (t->value)
		free(t->value);
	free(t);
}
