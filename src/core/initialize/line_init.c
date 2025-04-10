/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:43:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:24:29 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

/**
 * @brief シェルループの各ループ開始時の初期化
 *
 * @param shell シェル構造体へのポインタ
 */
void line_init(t_shell *shell)
{
    if (!shell)
        exit(1);
    xfree((void **)&shell->source_line);
    ft_lstclear(&shell->token_list, free_token);
    if (shell->ast)
    {
        free_ast(shell->ast);
        shell->ast = NULL;
    }
    xfree((void **)&shell->env_spc['?']);
    if (WIFSIGNALED(shell->status) || WIFEXITED(shell->status))
        shell->status = g_signal_status + 128;
    shell->env_spc['?'] = xitoa(shell->status);
    g_signal_status = 0;
    shell->status = 0;
}
