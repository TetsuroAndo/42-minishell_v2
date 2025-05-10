/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:12:00 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 15:15:57 by teando           ###   ########.fr       */
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
		status = mod_lex(sh) || mod_syn(sh) || mod_sem(sh, 1) || mod_exec(sh)
			|| 0;
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
	set_cloexec_all();
	sh = shell_init(env, av[0]);
	if (sh->debug & DEBUG_NO_PROMPT)
		shell_loop(sh, "");
	else
		shell_loop(sh, PROMPT);
	shell_exit(sh, sh->status);
}
