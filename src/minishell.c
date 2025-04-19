/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:12:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 08:50:35 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shell_loop(t_shell *shell, const char *prompt)
{
	int	status;

	while (42)
	{
		line_init(shell);
		shell->source_line = launch_readline(prompt);
		if (shell->source_line == NULL)
			shell_exit(shell, shell->status);
		if (g_signal_status == SIGINT)
		{
			g_signal_status = 0;
			free(shell->source_line);
			shell->source_line = NULL;
			continue ;
		}
		status = mod_lex(shell) || mod_syn(shell) || mod_sem(shell) || mod_exec(shell) || 0;
		if (status != E_NONE)
			printf("error status: %d\n", status);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*sh;

	(void)ac;
	if (init_signals() == -1)
		return (ft_dprintf(2, "signal setup failure\n"), 1);
	sh = shell_init(env, av[0]);
	if (sh->debug & DEBUG_CORE)
		ms_put_ascii(sh);
	shell_loop(sh, PROMPT);
	shell_exit(sh, sh->status);
}
