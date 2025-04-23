/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:10 by teando            #+#    #+#             */
/*   Updated: 2025/04/23 21:54:43 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"
#include "libms.h"

/**
 * @brief 環境変数特殊文字配列のメモリを解放する
 *
 * @param env_spc 環境変数特殊文字の配列
 */
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
	ft_lstclear(&shell->env_map, free);
	ft_lstclear(&shell->gcsh, free);
	free_env_spc(shell->env_spc);
	if (shell->stdin_backup != -1)
		xclose(&shell->stdin_backup);
	if (shell->stdout_backup != -1)
		xclose(&shell->stdout_backup);
	if (shell->stderr_backup != -1)
		xclose(&shell->stderr_backup);
	rl_clear_history();
}

/**
 * @brief シェルの終了処理
 *
 * @param shell シェル構造体へのポインタ
 * @param status 終了ステータス
 */
void	shell_exit(t_shell *shell, int status)
{
	if (shell->debug & DEBUG_CORE)
		ft_dprintf(STDERR_FILENO, "[SHELL_EXIT_STATUS]: %d\n", status);
	/*statusによっては終了までする必要ないときがある気がする*/
	shell_cleanup(shell);
	xfree((void **)&shell);
	exit(status);
}
