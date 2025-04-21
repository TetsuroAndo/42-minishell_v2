/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:12:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 18:35:10 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
			continue ;
		if (g_signal_status == SIGINT)
		{
			g_signal_status = 0;
			free(sh->source_line);
			sh->source_line = NULL;
			continue ;
		}
		status = mod_lex(sh) || mod_syn(sh) || mod_sem(sh) || mod_exec(sh) || 0;
		if (status != E_NONE && sh->debug & DEBUG_CORE)
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
	shell_loop(sh, PROMPT);
	shell_exit(sh, sh->status);
}
