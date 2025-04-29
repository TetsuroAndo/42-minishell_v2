/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_gc_track.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:27:03 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 19:29:30 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	ms_gc_track(void *p, t_shell *sh)
{
	if (!p || !sh)
		return (-1);
	ft_lstadd_front(&sh->gcli->next, xlstnew(p, sh));
	return (0);
}
