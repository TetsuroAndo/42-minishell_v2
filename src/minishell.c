/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:12:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 22:19:46 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shell_loop(t_shell *sh, const char *prompt)
{
	int	status;

	while (42)
	{
		line_init(sh);
		if (sh->debug & DEBUG_CORE)
			put_line_info(sh);
		sh->source_line = launch_readline(prompt);
		if (sh->source_line == NULL)
			shell_exit(sh, sh->status);
		if (g_signal_status == SIGINT)
		{
			xfree((void **)&sh->source_line);
			continue ;
		}
		status = mod_lex(sh) || mod_syn(sh) || mod_sem(sh,1) || mod_exec(sh) || 0;
		if (sh->debug & DEBUG_CORE && status != E_NONE)
			ft_dprintf(2, "[SH LOOP ERROR]: %d\n", status);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*sh;

	(void)ac;
	if (init_signals() == -1)
		return (ft_dprintf(2, "signal setup failure\n"), E_SYSTEM);
	sh = shell_init(env, av[0]);
	shell_loop(sh, PROMPT);
	shell_exit(sh, sh->status);
}
