/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_lex.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 02:00:01 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 03:26:57 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOD_LEX_H
# define MOD_LEX_H

# include "core.h"

t_status		mod_lex(t_shell *shell);

int				add_token(t_shell *shell, t_lexical_token *new_token);
t_lexical_token	*create_token(t_token_type type, char *value, t_shell *shell);
int				tokenize_line(t_shell *shell);
t_token_type	get_one_char_op(int c);
t_token_type	get_two_char_op(const char *s);
int				is_operator(const char *line, size_t pos);
char			*read_word(const char *line, size_t *pos, t_shell *shell);

int				validate_special_chars(const char *line, size_t *pos);
int				validate_redirect_missing_arg(const char *line, size_t *pos);

t_list			*handle_heredoc(t_lexical_token *tok, t_shell *sh);

void			debug_print_token_list(t_list *list);

#endif