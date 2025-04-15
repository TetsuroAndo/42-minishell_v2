/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:25:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/15 16:45:19 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

static void init_env(char **env, char *bin_name, t_shell *shell)
{
	shell->env_map = xlst_from_strs(env, shell);
	if (!shell->env_map)
		shell_exit(shell, E_ALLOCATE);
	shell->env_spc['?'] = xitoa(0, shell);
	if (!shell->env_spc['?'])
		shell_exit(shell, E_ALLOCATE);
	shell->env_spc['\0'] = ms_strdup(bin_name, shell);
	if(!shell->env_spc['\0'])
		shell_exit(shell, E_ALLOCATE);
}

static void init_fd_backup(t_shell *shell)
{
	shell->stdin_backup = xdup(STDIN_FILENO, shell);
	shell->stdout_backup = xdup(STDOUT_FILENO, shell);
	shell->stderr_backup = xdup(STDERR_FILENO, shell);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1 || shell->stderr_backup == -1)
		shell_exit(shell, E_SYSTEM);
}

/**
 * @brief シェル構造体の初期化
 *
 * @param env 環境変数配列
 * @return t_shell* 成功時シェル構造体へのポインタ、エラー時NULL
 */
t_shell *shell_init(char **env, char *bin_name)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		shell_exit(NULL, E_ALLOCATE);
	shell->bin_name = bin_name;
	init_env(env, bin_name, shell);
	init_fd_backup(shell);
	if (getcwd(shell->cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		shell_exit(shell, E_SYSTEM);
	}
	shell->status = E_NONE;
	shell->exit_flag = 0;
	shell->interactive = isatty(STDIN_FILENO);
	shell->debug = DEFAULT_DEBUG;
	return (shell);
}
