/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/22 09:14:23 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"
# include "libft.h"

typedef struct s_shell	t_shell;
typedef enum e_status	t_status;
typedef struct s_ast	t_ast;

/*
** ============================================================================
** ms_gc - ガベージコレクション関連の関数
** ============================================================================
*/

void					*xmalloc_gcshell(size_t size, t_shell *shell);
void					*xmalloc_gcline(size_t size, t_shell *shell);
char					*ms_strndup_gcshell(const char *s, size_t n,
							t_shell *sh);
char					*ms_strdup_gcshell(const char *s, t_shell *sh);
char					*ms_substr_gcshell(char const *s, unsigned int start,
							size_t len, t_shell *sh);
char					*ms_substr_r_gcshell(char const *s, char delimiter,
							t_shell *sh);
char					*ms_substr_l_gcshell(const char *s, char delimiter,
							t_shell *sh);

/*
** ============================================================================
** ms_lexical - トークン関連の関数
** ============================================================================
*/
void					free_token(void *token);

/*
** ============================================================================
** ms_env - 環境変数関連の関数
** ============================================================================
*/
// key
int						ms_isactivekey(const char *key);
t_status				ms_partenvarg(char *key, const char *arg, int *is_append,
						size_t *eq_pos);

// value
int						ms_isactivevalue(const char *value);
char					*ms_escapevalue(const char *value, t_shell *shell);

int						ms_envcmp(void *data, void *key);
char					*ms_getenv(const char *key, t_shell *shell);
t_status				ms_setenv(char *arg, t_shell *shell);
t_status				ms_setenv_item(const char *key, const char *value,
							t_shell *shell);
t_status				ms_unset(const char *key, t_shell *shell);

/*
** ============================================================================
** ms_ast- 抽象構文木関連の関数
** ============================================================================
*/
void					free_ast(t_ast **ast);

/*
** ============================================================================
** ms_fd - ファイルディスクリプタ関連の関数
** ============================================================================
*/
int						xclose(int *fd);
int						xdup(int oldfd, t_shell *shell);
int						xdup2(int *oldfd, int newfd, t_shell *shell);

/*
** ============================================================================
** ms_path - パス解決関連の関数
** ============================================================================
*/

int						is_builtin(char *cmd);
int						path_home(char path[], const char *src, int mode,
							t_shell *shell);
int						path_relative(char path[], const char *src, int mode,
							t_shell *shell);
int						path_resolve(char **in, t_shell *shell);

/*
** ============================================================================
** ms_system - システム関連の関数
** ============================================================================
*/
pid_t					xfork(t_shell *shell);
void					xfree(void **ptr);
void					*xmalloc(size_t size, t_shell *shell);
int						xpipe(int pipfds[], t_shell *shell);

/*
** ============================================================================
** ms_string - 文字列操作関連の関数
** ============================================================================
*/
int						is_quoted(const char *s);
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
char						*xstrjoin(char const *s1, char const *s2,
							t_shell *shell);
size_t					ms_path_cleancpy(char *dst, const char *src, size_t siz);
size_t					ms_path_cleancat(char *dst, const char *s, size_t siz);
char					*xstrjoin3(char const *s1, char const *s2,
							char const *s3, t_shell *shell);
char					*xstrjoin_free(char const *s1, char const *s2,
							t_shell *shell);
char					*xstrjoin_free2(char const *s1, char const *s2,
							t_shell *shell);

/*
** ============================================================================
** ms_lst - lst操作関連の関数
** ============================================================================
*/
void					*ms_listshift(t_list **list);
int						ms_lstiter(t_list *lst, int (*f)(t_list **, void *, int,
								t_shell *), t_shell *shell);
t_list					*xlstnew(void *data, t_shell *shell);
t_list					*xlst_from_strs(char **strs, t_shell *shell);
char					**xlst_to_strs(t_list *lst, t_shell *shell);
t_list					*ms_lstcopy(t_list *lst, void (*del)(void *),
							t_shell *shell);

#endif
