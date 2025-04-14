/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:10 by teando            #+#    #+#             */
/*   Updated: 2025/04/14 20:27:20 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	free_env_spc(shell->env_spc);
	if (shell->stdin_backup != -1)
		xclose(&shell->stdin_backup);
	if (shell->stdout_backup != -1)
		xclose(&shell->stdout_backup);
	if (shell->stderr_backup != -1)
		xclose(&shell->stderr_backup);
}

/**
 * @brief シェルの終了処理
 *
 * @param shell シェル構造体へのポインタ
 * @param status 終了ステータス
 */
void	shell_exit(t_shell *shell, int status)
{
	printf("shell exit with status %d\n", status);
	/*statusによっては終了までする必要ないときがある気がする*/
	shell_cleanup(shell);
	xfree((void **)&shell);
	exit(status);
}
