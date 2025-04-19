/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_put_ascii.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 08:39:43 by teando            #+#    #+#             */
/*   Updated: 2025/04/19 08:49:30 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static int check_eof(char *s, t_shell *sh)
{
	int chk;

	chk = ft_putendl_fd(s, STDOUT_FILENO);
	if (chk == EOF)
		shell_exit(sh, E_SYSTEM);
	return (chk);
}

void	ms_put_ascii(t_shell *sh)
{
	check_eof("▄▄▄█████▓ ██▀███   ▄▄▄        ██████  ██░ ██ ", sh);
	check_eof("▓  ██▒ ▓▒▓██ ▒ ██▒▒████▄    ▒██    ▒ ▓██░ ██▒", sh);
	check_eof("▒ ▓██░ ▒░▓██ ░▄█ ▒▒██  ▀█▄  ░ ▓██▄   ▒██▀▀██░", sh);
	check_eof("░ ▓██▓ ░ ▒██▀▀█▄  ░██▄▄▄▄██   ▒   ██▒░▓█ ░██ ", sh);
	check_eof("  ▒██▒ ░ ░██▓ ▒██▒ ▓█   ▓██▒▒██████▒▒░▓█▒░██▓", sh);
	check_eof("  ▒ ░░   ░ ▒▓ ░▒▓░ ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒", sh);
	check_eof("    ░      ░▒ ░ ▒░  ▒   ▒▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░", sh);
	check_eof("  ░        ░░   ░   ░   ▒   ░  ░  ░   ░  ░░ ░", sh);
	check_eof("            ░           ░  ░      ░   ░  ░  ░", sh);
}
