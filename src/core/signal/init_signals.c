/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:45:12 by teando            #+#    #+#             */
/*   Updated: 2025/04/24 01:33:55 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

volatile sig_atomic_t g_signal_status = 0;

static int rl_noob(void)
{
	return (0);
}

static void sigint_handler(int signum)
{
    if (signum == SIGINT)
    {
        g_signal_status = SIGINT;
        rl_done = 1;
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

/**
 * @brief SIGQUIT (Ctrl+\) シグナルハンドラー
 *
 * @param signum シグナル番号
 */
// static void sigquit_handler(int signum)
// {
//     if (signum == SIGQUIT)
//     {
//         g_signal_status = SIGQUIT;
//         rl_done = 1;
//     }
// }

/**
 * @brief シグナルハンドラーの初期化
 *
 * @return int 成功時0、エラー時-1
 */
int init_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    rl_event_hook = (rl_hook_func_t *)rl_noob;
    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        return (-1);
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = SA_RESTART;
    sigemptyset(&sa_quit.sa_mask);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        return (-1);
    g_signal_status = 0;
    return (0);
}
