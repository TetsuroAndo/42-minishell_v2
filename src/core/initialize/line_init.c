/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:43:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/20 07:42:33 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

/**
 * @brief シェルループの各ループ開始時の初期化
 *
 * @param shell シェル構造体へのポインタ
 */
void	line_init(t_shell *sh)
{
	if (!sh)
		shell_exit(NULL, 1);
	xfree((void **)&sh->source_line);
	if (sh->token_list)
		ft_lstclear(&sh->token_list, free_token);
	if (sh->ast)
		free_ast(&sh->ast);
	ft_lstclear(&sh->gcli, free);
	xfree((void **)&sh->env_spc['?']);
	if (WIFSIGNALED(sh->status))
		sh->status = g_signal_status + 128;
	else if (WIFEXITED(sh->status))
		sh->status = WEXITSTATUS(sh->status);
	sh->env_spc['?'] = xitoa(sh->status, sh);
	g_signal_status = 0;
	sh->status = 0;
	if (sh->debug & DEBUG_CORE)
		put_line_init(sh);
}
