/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:43:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/21 01:04:52 by tomsato          ###   ########.fr       */
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
	sh->env_spc['?'] = xitoa(sh->status, sh);
	g_signal_status = 0;
	sh->status = 0;
	if (sh->debug & DEBUG_CORE)
		put_line_init(sh);
}
