/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   true.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:56:51 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 15:58:10 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmds.h"

t_status	__true(int argc, char **argv, t_shell *sh)
{
	(void) argc;
	(void) argv;
	(void) sh;
	return (E_NONE);
}
