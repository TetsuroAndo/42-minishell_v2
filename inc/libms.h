/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libms.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:49:25 by teando            #+#    #+#             */
/*   Updated: 2025/04/15 19:55:51 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LIBMS_H
# define LIBMS_H

# include "core.h"
# include "libft.h"

typedef struct s_shell	t_shell;
typedef struct s_ast	t_ast;
typedef enum e_status	t_status;

/*
** ============================================================================
** ms_env - 環境変数関連の関数
** ============================================================================
*/
int						ms_isactivekey(const char *key);
int						ms_envcmp(void *data, void *key);
char					*ms_getenv(const char *key, t_shell *shell);
t_status				ms_partenvarg(char *key, const char *arg);
t_status				ms_setenv(const char *arg, t_shell *shell);
t_status				ms_setenv_item(const char *key, const char *value,
							t_shell *shell);
t_status				ms_unset(const char *key, t_shell *shell);

/*
** ============================================================================
** ms_free - メモリ解放関連の関数
** ============================================================================
*/
void					xfree(void **ptr);
void					free_ast(t_ast **ast);
void					free_token(void *token);

/*
** ============================================================================
** ms_fd - ファイルディスクリプタ関連の関数
** ============================================================================
*/
int						xclose(int *fd);
int						xdup(int oldfd, t_shell *shell);
int						xdup2(int oldfd, int newfd, t_shell *shell);

/*
** ============================================================================
** ms_path - パス解決関連の関数
** ============================================================================
*/
int						path_home(char path[], const char *src, int mode,
							t_shell *shell);
int						path_relative(char path[], const char *src, int mode,
							t_shell *shell);
char					*path_resolve(const char *cmd, t_shell *shell);

/*
** ============================================================================
** ms_process - プロセス関連の関数
** ============================================================================
*/
pid_t					xfork(t_shell *shell);
void					*xmalloc(size_t size, t_shell *shell);
int						xpipe(int pipfds[], t_shell *shell);

/*
** ============================================================================
** ms_string - 文字列操作関連の関数
** ============================================================================
*/
int ms_lstiter(t_list *lst, int (*f)(void *, int, t_shell *), t_shell *shell);
void skip_spaces(const char *line, size_t *pos);
char					*ms_strndup(const char *s, size_t n, t_shell *shell);
char					*ms_strdup(const char *s, t_shell *shell);
char					*ms_substr(char const *s, unsigned int start,
							size_t len, t_shell *shell);
char					*ms_substr_r(char const *s, char delimiter,
							t_shell *shell);
char					*ms_substr_l(char const *s, char delimiter,
							t_shell *shell);
char					*xitoa(int n, t_shell *shell);
t_list					*xlstnew(void *data, t_shell *shell);
t_list					*xlst_from_strs(char **strs, t_shell *shell);
char					**xlst_to_strs(t_list *lst, t_shell *shell);
char					**xsplit(char *str, char sep, t_shell *shell);
char					*xstrjoin_free(char const *s1, char const *s2,
							t_shell *shell);

#endif
