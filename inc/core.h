/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 08:23:28 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:20:43 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "libft.h"
# include "libms.h"
# include "ms_ast.h"
# include "ms_debug.h"
# include "ms_err.h"
# include "ms_readline.h"
# include "ms_signal.h"
# include "ms_token.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef DEBUG_MODE
#  define DEBUG_MODE DEBUG_NONE
# endif

extern volatile sig_atomic_t	g_signal_status;

typedef struct s_shell
{
	char						*bin_name;

	char						*source_line;
	t_list						*token_list;
	t_list						*token_list_syn;
	t_ast						*ast;

	t_status					status;
	int							exit_flag;

	t_list						*env_map;
	char						*env_spc[128];
	char						cwd[PATH_MAX];
	int							env_updated;

	int							stdin_backup;
	int							stdout_backup;
	int							stderr_backup;

	t_list						*gcli;

	int							interactive;
	int							debug;
}								t_shell;

/*
** シェルの初期化と終了処理
*/
void							line_init(t_shell *shell);
t_shell							*shell_init(char **env, char *bin_name);
void							shell_cleanup(t_shell *shell);
void							shell_exit(t_shell *shell, int status);

#endif
