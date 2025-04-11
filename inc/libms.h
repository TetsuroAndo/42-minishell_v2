/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 21:03:53 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"

typedef struct s_shell	t_shell;
typedef struct s_ast	t_ast;

// ms_free
void					xfree(void **ptr);
void					free_ast(t_ast **ast);
void					free_token(void *token);

// ms_string
char					*ms_strndup(const char *s, size_t n, t_shell *shell);
char					*ms_strdup(const char *s, t_shell *shell);
char					*ms_substr(char const *s, unsigned int start,
							size_t len, t_shell *shell);
void					skip_spaces(const char *line, size_t *pos);

// ms_rappers
int						xclose(int *fd);
int						xdup(int oldfd, t_shell *shell);
int						xdup2(int oldfd, int newfd, t_shell *shell);
pid_t					xfork(t_shell *shell);
void					*xmalloc(size_t size, t_shell *shell);
t_list					*xlstnew(char *data, t_shell *shell);
char					*xitoa(int n, t_shell *shell);
int						xpipe(int pipfds[], t_shell *shell);
t_list					*xlst_from_strs(char **strs, t_shell *shell);
char					**xlst_to_strs(t_list *lst, t_shell *shell);
char					**xsplit(char *str, char sep, t_shell *shell);
char					*xstrjoin_free(char const *s1, char const *s2,
							t_shell *shell);

#endif
