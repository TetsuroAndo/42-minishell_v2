/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xdup.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:18:09 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 00:58:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int	xdup(int oldfd, t_shell *shell)
{
	int	newfd;

	newfd = dup(oldfd);
	if (newfd == -1)
	{
		perror("dup");
		shell_exit(shell, errno);
	}
	return (newfd);
}

int	xdup2(int oldfd, int newfd, t_shell *shell)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror("dup2");
		shell_exit(shell, errno);
	}
	xclose(&oldfd);
	return (newfd);
}
