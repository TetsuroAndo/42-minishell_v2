/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:12:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/18 19:51:09 by teando           ###   ########.fr       */
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
		status = mod_lex(shell) || mod_syn(shell) || mod_sem(shell);
		if (status != E_NONE)
			printf("error status: %d\n", status);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;

	(void)ac;
	if (init_signals() == -1)
		return (ft_dprintf(2, "signal setup failure\n"), 1);
	shell = shell_init(env, av[0]);
	shell_loop(shell, PROMPT);
	shell_exit(shell, shell->status);
}
