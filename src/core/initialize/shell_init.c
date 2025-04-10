/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:25:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 01:05:58 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

/**
 * @brief シェル構造体の初期化
 *
 * @param env 環境変数配列
 * @return t_shell* 成功時シェル構造体へのポインタ、エラー時NULL
 */
t_shell *shell_init(char **env)
{
	t_shell	*shell;

	shell = ft_calloc(sizeof(t_shell), 1);
	if (!shell)
		shell_exit(NULL, 1);
	shell->exit_flag = 0;
	shell->env_map = xlst_from_strs(env, shell);
	if (!shell->env_map)
		shell_exit(shell, 1);
	if (getcwd(shell->cwd, PATH_MAX) == NULL)
	{
		perror("pwd");
		shell_exit(shell, 1);
	}
	shell->env_spc['?'] = xitoa(0, shell);
	if (!shell->env_spc['?'])
		shell_exit(shell, 1);
	shell->interactive = isatty(STDIN_FILENO);
	shell->stdin_backup = xdup(STDIN_FILENO, shell);
	shell->stdout_backup = xdup(STDOUT_FILENO, shell);
	shell->stderr_backup = xdup(STDERR_FILENO, shell);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1 || shell->stderr_backup == -1)
		shell_exit(shell, 1);
	shell->debug = DEFAULT_DEBUG;
	return (shell);
}
