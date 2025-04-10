/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:10 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 19:42:48 by tomsato          ###   ########.fr       */
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

/**
 * @brief シェルのクリーンアップ処理
 *
 * @param shell シェル構造体へのポインタ
 */
void	shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	// analize
	if (shell->source_line)
		xfree(&shell->source_line);
	if (shell->token_list)
	{
		ft_lstclear(&shell->token_list, clear_data);
		shell->token_list = NULL;
	}
	if ()
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
	exit(status);
}
