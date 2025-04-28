/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xfork.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:58:56 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 21:12:05 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

pid_t	xfork(t_shell *shell)
{
	pid_t	pid;
	int		retry;

	pid = fork();
	if (pid == -1)
	{
		retry = 0;
		while (retry++ < 5)
		{
			pid = fork();
			if (pid != -1)
				return (pid);
		}
		perror("fork");
		shell_exit(shell, errno);
	}
	return (pid);
}
