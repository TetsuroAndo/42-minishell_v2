/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 06:26:03 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:37:49 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_DEBUG_H
# define MS_DEBUG_H

typedef struct s_shell	t_shell;

# define DEBUG_NONE 0x00
# define DEBUG_CORE 0x01
# define DEBUG_LEX 0x02
# define DEBUG_SYN 0x04
# define DEBUG_SEM 0x08
# define DEBUG_ENV 0x10
# define DEBUG_EXEC 0x20
# define DEBUG_REDR 0x40
# define DEBUG_SIGNAL 0x80
# define DEBUG_ALL 0xFF

void					put_sh_init(t_shell *sh);
void					put_line_info(t_shell *sh);
void					put_line_after(t_shell *sh);
void					put_line_before(t_shell *sh);
void					put_sh_final(t_shell *sh, int status);

#endif
