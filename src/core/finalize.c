/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:10 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:14:27 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "libms.h"

static void	free_env_spc(char **env_spc)
{
	size_t	i;
	void	*temp;

	i = 0;
	while (i < 128)
	{
		if (env_spc[i])
		{
			temp = env_spc[i];
			xfree((void **)&temp);
			env_spc[i] = NULL;
		}
		i++;
	}
}

void	shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	line_init(shell);
	ft_lstclear(&shell->env_map, free);
	ft_gc_destroy(&shell->gcli);
	free_env_spc(shell->env_spc);
	if (shell->stdin_backup != -1)
		xclose(&shell->stdin_backup);
	if (shell->stdout_backup != -1)
		xclose(&shell->stdout_backup);
	if (shell->stderr_backup != -1)
		xclose(&shell->stderr_backup);
	rl_clear_history();
}

void	shell_exit(t_shell *shell, int status)
{
	if (shell->debug & DEBUG_CORE)
		put_sh_final(shell, status);
	shell_cleanup(shell);
	xfree((void **)&shell);
	exit(status);
}
