/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 06:26:03 by teando            #+#    #+#             */
/*   Updated: 2025/04/25 13:00:17 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_DEBUG_H
# define MS_DEBUG_H

typedef struct s_shell	t_shell;

# define DEBUG_NONE 0            // 00000000
# define DEBUG_CORE (1 << 0)     // 00000001
# define DEBUG_LEX (1 << 1)      // 00000010
# define DEBUG_SYN (1 << 2)      // 00000100
# define DEBUG_SEM (1 << 3)      // 00001000
# define DEBUG_ENV (1 << 4)      // 00010000
# define DEBUG_EXEC (1 << 5)     // 00100000
# define DEBUG_REDIRECT (1 << 6) // 01000000
# define DEBUG_SIGNAL (1 << 7)   // 10000000
# define DEBUG_ALL (~DEBUG_NONE) // 全てのビットを立てる (~0 と同じ)

void	put_sh_init(t_shell *sh);
void	put_line_info(t_shell *sh);
void	put_line_after(t_shell *sh);
void	put_line_before(t_shell *sh);
void	put_sh_final(t_shell *sh, int status);

#endif
