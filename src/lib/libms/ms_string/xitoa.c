/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xitoa.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:52:07 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 17:30:46 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libms.h"

char	*xitoa(int n, t_shell *shell)
{
	char	*ans;

	ans = ft_itoa(n);
	if (!ans)
		shell_exit(shell, E_SYSTEM);
	return (ans);
}
