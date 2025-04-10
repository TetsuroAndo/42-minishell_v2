/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:43:20 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 21:39:22 by teando           ###   ########.fr       */
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
        shell_exit(NULL, 1);
    xfree((void **)&shell->source_line);
    ft_lstclear(&shell->token_list, free_token);
    if (shell->ast)
        free_ast(&shell->ast);
    xfree((void **)&shell->env_spc['?']);
    if (WIFSIGNALED(shell->status))
        shell->status = g_signal_status + 128;
    else if (WIFEXITED(shell->status))
        shell->status = WEXITSTATUS(shell->status);
    shell->env_spc['?'] = xitoa(shell->status, shell);
    g_signal_status = 0;
    shell->status = 0;
}
