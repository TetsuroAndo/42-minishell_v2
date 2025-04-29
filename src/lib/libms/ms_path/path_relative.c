/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_relative.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 02:11:59 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 07:54:51 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libms.h"

static void	__handle_dot(char **src_ptr)
{
	const char	*src;

	src = *src_ptr;
	if (src[1])
		*src_ptr += 2;
	else
		*src_ptr += 1;
}

static void	__handle_dotdot(char **dst_ptr, char **src_ptr, char *path)
{
	char	*dst;

	dst = *dst_ptr;
	if (dst > path + 1)
	{
		dst--;
		while (dst > path && dst[-1] != '/')
			dst--;
	}
	*dst = '\0';
	if ((*src_ptr)[2])
		*src_ptr += 3;
	else
		*src_ptr += 2;
	*dst_ptr = dst;
}

static void	__copy_token(char **dst_ptr, char **src_ptr)
{
	char	*dst;
	char	*src;

	dst = *dst_ptr;
	src = *src_ptr;
	while (*src && *src != '/')
		*dst++ = *src++;
	if (*src == '/')
		*dst++ = *src++;
	*dst_ptr = dst;
	*src_ptr = src;
}

static void	__init_path(char path[], const char *src, t_shell *sh,
		char **dst_ptr)
{
	char	*dst;

	(void)src;
	ms_path_cleancpy(path, sh->cwd, PATH_MAX + 1);
	dst = path + ft_strlen(path);
	if (dst > path && dst[-1] != '/')
		*dst++ = '/';
	*dst_ptr = dst;
}

int	path_relative(char path[], const char *src, int mode, t_shell *sh)
{
	char	*dst;
	char	*src_ptr;

	__init_path(path, src, sh, &dst);
	src_ptr = (char *)src;
	while (*src_ptr)
	{
		if (*src_ptr == '/')
			++src_ptr;
		else if (*src_ptr == '.' && (src_ptr[1] == '/' || src_ptr[1] == '\0'))
			__handle_dot(&src_ptr);
		else if (*src_ptr == '.' && src_ptr[1] == '.' && (src_ptr[2] == '/'
				|| src_ptr[2] == '\0'))
			__handle_dotdot(&dst, &src_ptr, path);
		else
			__copy_token(&dst, &src_ptr);
	}
	if (dst > path + 1 && dst[-1] == '/')
		dst--;
	*dst = '\0';
	return (access(path, mode));
}
