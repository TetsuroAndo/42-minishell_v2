/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:25:22 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 11:04:27 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

static void	init_env(char **env, char *bin_name, t_shell *sh)
{
	sh->env_map = xlst_from_strs(env, sh);
	if (!sh->env_map)
		shell_exit(sh, E_SYSTEM);
	sh->env_spc['?'] = xitoa(0, sh);
	if (!sh->env_spc['?'])
		shell_exit(sh, E_SYSTEM);
	sh->env_spc['\0'] = ms_strdup(bin_name, sh);
	if (!sh->env_spc['\0'])
		shell_exit(sh, E_SYSTEM);
}

static void	init_fd_backup(t_shell *sh)
{
	sh->stdin_backup = xdup(STDIN_FILENO, sh);
	sh->stdout_backup = xdup(STDOUT_FILENO, sh);
	sh->stderr_backup = xdup(STDERR_FILENO, sh);
	if (sh->stdin_backup == -1 || sh->stdout_backup == -1
		|| sh->stderr_backup == -1)
		shell_exit(sh, E_SYSTEM);
}

/**
 * @brief シェル構造体の初期化
 *
 * @param env 環境変数配列
 * @return t_shell* 成功時シェル構造体へのポインタ、エラー時NULL
 */
t_shell	*shell_init(char **env, char *bin_name)
{
	t_shell	*sh;

	sh = ft_calloc(sizeof(t_shell), 1);
	if (!sh)
		shell_exit(NULL, E_SYSTEM);
	sh->bin_name = bin_name;
	init_env(env, bin_name, sh);
	init_fd_backup(sh);
	if (getcwd(sh->cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		shell_exit(sh, E_SYSTEM);
	}
	sh->env_updated = 0;
	sh->gcli = xlstnew(NULL, sh);
	sh->status = E_NONE;
	sh->exit_flag = 0;
	sh->interactive = isatty(STDIN_FILENO);
	sh->debug = DEBUG_MODE;
	if (sh->debug != DEBUG_NONE)
		put_sh_init(sh);
	return (sh);
}
