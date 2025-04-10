/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:10 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:46:03 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "libms.h"

// #include "mod_env.h"

/**
 * @brief ft_lstclearに渡すdel関数
 *
 * @param t_listのdata
 */
static void	clear_data(void *data)
{
	xfree(&data);
	return ;
}

static void free_env_spc(char **env_spc)
{
	size_t	i;

	i = 0;
	while (i < 128)
	{
		if (env_spc[i])
			xfree(&env_spc[i]);
		i++;
	}
	return ;
}

/**
 * @brief シェルのクリーンアップ処理
 *
 * @param shell シェル構造体へのポインタ
 */
void	shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	line_init(shell);
	ft_lstclear(&shell->env_map, clear_data);
	free_env_spc(shell->env_spc);
	xfree(&shell->cwd);
	if (shell->stdin_backup != -1)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup != -1)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
	if (shell->stderr_backup != -1)
	{
		close(shell->stderr_backup);
		shell->stderr_backup = -1;
	}
}

/**
 * @brief シェルの終了処理
 *
 * @param shell シェル構造体へのポインタ
 * @param status 終了ステータス
 */
void	shell_exit(t_shell *shell, int status)
{
	shell_cleanup(shell);
	xfree(&shell);
	exit(status);
}
