/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sh_proc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 13:24:52 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 12:03:28 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

static char	*put_debug_mode(t_shell *sh)
{
	if (sh->debug == DEBUG_CORE)
		return ("CORE");
	else if (sh->debug == DEBUG_LEX)
		return ("LEX");
	else if (sh->debug == DEBUG_SYN)
		return ("SYNTAX");
	else if (sh->debug == DEBUG_SEM)
		return ("SEMANTIC");
	else if (sh->debug == DEBUG_ENV)
		return ("ENV");
	else if (sh->debug == DEBUG_EXEC)
		return ("EXEC");
	else if (sh->debug == DEBUG_REDR)
		return ("REDIRECT");
	else if (sh->debug == DEBUG_SIGNAL)
		return ("SIGNAL");
	else if (sh->debug == DEBUG_ALL)
		return ("ALL");
	else
		return ("NONE");
}

void	put_sh_init(t_shell *sh)
{
	const char	pid[36] = "This project does not use getpid().";

	ms_put_ascii(sh);
	ft_dprintf(STDERR_FILENO, "-----> [SHELL NAME]: %s\n", sh->bin_name);
	ft_dprintf(STDERR_FILENO, "[PID]: %s\n", pid);
	ft_dprintf(STDERR_FILENO, "[DEBUG_MODE]: %s\n", put_debug_mode(sh));
	ft_dprintf(STDERR_FILENO, "[INTERACTIVE/TTY]: %d\n", sh->interactive);
}
