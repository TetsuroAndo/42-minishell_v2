/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:40:08 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/19 04:29:15 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

t_status	mod_exec(t_shell *sh)
{
	int	st;

	if (!sh || !sh->ast)
		return (E_NONE);
	st = exe_run(sh->ast, sh);
	sh->status = st;
	if (sh->debug & DEBUG_EXEC)
	{
		if (st != 0)
			printf("mod_exec Error: %d\n", st);
	}
	return (E_NONE);
}
