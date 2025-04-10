/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpipe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:43:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 01:01:32 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

int xpipe(int pipfds[], t_shell *shell)
{
	if (pipe(pipfds) == -1)
	{
		perror("pipe");
		shell_exit(shell, errno);
	}
	return (0);
}
