/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:25:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:46:13 by teando           ###   ########.fr       */
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
		system_exit(NULL, 1);
	shell->exit_flag = 0;
	shell->env_map = ft_list_from_strs(env);
	if (!shell->env_map)
		system_exit(shell, 1);
	if (getcwd(shell->cwd, PATH_MAX) == NULL)
	{
		perror("pwd");
		system_exit(shell, 1);
	}
	shell->env_spc['?'] = xitoa(0);
	if (!shell->env_spc['?'])
		system_exit(shell, 1);
	shell->interactive = isatty(STDIN_FILENO);
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	shell->stderr_backup = dup(STDERR_FILENO);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1 || shell->stderr_backup == -1)
		system_exit(shell, 1);
	return (shell);
}
