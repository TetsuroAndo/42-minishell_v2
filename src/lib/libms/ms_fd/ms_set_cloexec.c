/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_set_cloexec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:05:57 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 22:15:53 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

void	ms_set_cloexec(int fd, t_shell *sh)
{
	if (fd < 0)
		return ;
	if (ioctl(fd, FIOCLEX) == -1)
	{
		perror("ioctl(FIOCLEX)");
		if (!sh)
			exit(errno);
		shell_exit(sh, errno);
	}
}
