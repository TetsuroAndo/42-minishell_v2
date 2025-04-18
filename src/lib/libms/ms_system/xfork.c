/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xfork.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:58:56 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 00:59:16 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

pid_t	xfork(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell_exit(shell, errno);
	}
	return (pid);
}
