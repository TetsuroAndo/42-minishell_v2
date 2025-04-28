/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xclose.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:15:46 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:07:46 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

int	xclose(int *fd)
{
	int	ret;

	if (!fd || *fd < 3)
		return (0);
	ret = close(*fd);
	if (ret == -1)
		perror("close");
	*fd = -1;
	return (ret);
}
