/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/10 20:55:06 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"

// ms_free
void	xfree(void **ptr);
void    free_ast(t_ast *ast);
void    free_token(void *token);

// ms_string
void    skip_spaces(const char *line, size_t *pos);

// ms_rappers
int		xclose(int *fd);
char	*xdup(char *str, t_shell *shell);
char	*xdup2(char *str, t_shell *shell);
void	*xmalloc(size_t size, t_shell *shell);
t_list	*xlstnew(char *data, t_shell *shell);
char    *xitoa(int n, t_shell *shell);

#endif
