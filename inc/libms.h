/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 19:32:02 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"

int		xclose(int *fd);
char	*xdup(char *str, t_shell shell);
char	*xdup2(char *str, t_shell shell);
void	xfree(void **ptr);
void	*xmalloc(size_t size, t_shell *shell);
t_list	*xlstnew(char *data, t_shell *shell);

#endif
