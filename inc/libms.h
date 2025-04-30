/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/30 12:15:41 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"
# include "libft.h"

typedef struct s_shell	t_shell;
typedef enum e_status	t_status;
typedef struct s_ast	t_ast;

int						ms_gc_track(void *p, t_shell *sh);
void					xfree_gc(void **p, t_shell *sh);
void					*xmalloc_gcline(size_t size, t_shell *shell);
t_list					*xlstnew_gcli(void *data, t_shell *sh);
char					*ms_strjoin_gcli(char const *s1, char const *s2,
							t_shell *sh);
char					*ms_strjoin3_gcli(char const *s1, char const *s2,
							char const *s3, t_shell *sh);
char					*ms_strjoin_gcli_f2(char const *s1, char const *s2,
							t_shell *sh);
char					*ms_strndup_gcli(const char *s, size_t n, t_shell *sh);
char					*ms_strdup_gcli(const char *s, t_shell *sh);
char					*ms_substr_gcli(char const *s, unsigned int start,
							size_t len, t_shell *sh);
char					*ms_substr_r_gcli(char const *s, char delimiter,
							t_shell *sh);
char					*ms_substr_l_gcli(const char *s, char delimiter,
							t_shell *sh);

void					free_token(void *token);
void					*copy_token(void *data, t_shell *sh);

int						ms_isactivekey(const char *key);
t_status				ms_partenvarg(char *key, const char *arg,
							int *is_append, size_t *eq_pos);

int						ms_isactivevalue(const char *value);
char					*ms_escapevalue(const char *value, t_shell *shell);

int						ms_envcmp(void *data, void *key);
char					*ms_getenv(const char *key, t_shell *shell);
t_status				ms_setenv(char *arg, t_shell *shell);
t_status				ms_setenv_item(const char *key, const char *value,
							t_shell *shell);
t_status				ms_unset(const char *key, t_shell *shell);

void					free_ast(t_ast **ast);

int						xclose(int *fd);
int						xdup(int oldfd, t_shell *shell);
int						xdup2(int *oldfd, int newfd, t_shell *shell);

int						is_builtin(char *cmd);
int						path_home(char path[], const char *src, int mode,
							t_shell *shell);
int						path_relative(char path[], const char *src, int mode,
							t_shell *shell);
int						path_resolve(char **in, t_shell *shell);

pid_t					xfork(t_shell *shell);
void					xfree(void **ptr);
void					*xmalloc(size_t size, t_shell *shell);
int						xpipe(int pipfds[], t_shell *shell);
void					set_cloexec_all(void);
void					ms_set_cloexec(int fd, t_shell *sh);

int						is_quoted(const char *s);
int						skip_quoted_word(const char *line, size_t *pos,
							t_shell *shell);
void					ms_put_ascii(t_shell *sh);
void					skip_spaces(const char *line, size_t *pos);
void					skip_dollar_paren(const char *line, size_t *pos);
char					*ms_strndup(const char *s, size_t n, t_shell *shell);
char					*ms_strdup(const char *s, t_shell *shell);
char					*ms_substr(char const *s, unsigned int start,
							size_t len, t_shell *shell);
char					*ms_substr_r(char const *s, char delimiter,
							t_shell *shell);
char					*ms_substr_l(char const *s, char delimiter,
							t_shell *shell);
char					*trim_valid_quotes(const char *s, t_shell *sh);
char					*xitoa(int n, t_shell *shell);
char					**xsplit(char *str, char sep, t_shell *shell);
char					*xstrjoin(char const *s1, char const *s2,
							t_shell *shell);
size_t					ms_path_cleancpy(char *dst, const char *src,
							size_t siz);
size_t					ms_path_cleancat(char *dst, const char *s, size_t siz);
char					*xstrjoin3(char const *s1, char const *s2,
							char const *s3, t_shell *shell);
char					*xstrjoin_free(char const *s1, char const *s2,
							t_shell *shell);
char					*xstrjoin_free2(char const *s1, char const *s2,
							t_shell *shell);

void					*ms_listshift(t_list **list);
int						ms_lstiter(t_list *lst, int (*f)(t_list **, int,
								t_shell *), t_shell *shell);
t_list					*xlstnew(void *data, t_shell *shell);
t_list					*xlst_from_strs(char **strs, t_shell *shell);
char					**xlst_to_strs(t_list *lst, t_shell *shell);
t_list					*ms_lstcopy(t_list *lst, void (*del)(void *),
							t_shell *shell);

#endif
