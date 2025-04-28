/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_lex.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 02:00:01 by teando            #+#    #+#             */
/*   Updated: 2025/04/29 02:00:34 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOD_LEX_H
# define MOD_LEX_H

# include "core.h"

/* entry point for lexical analysis */
t_status		mod_lex(t_shell *shell);

// Tokenization functions
int				add_token(t_shell *shell, t_lexical_token *new_token);
t_lexical_token	*create_token(t_token_type type, char *value, t_shell *shell);
int				tokenize_line(t_shell *shell);
t_token_type	get_one_char_op(int c);
t_token_type	get_two_char_op(const char *s);
int				is_operator(const char *line, size_t pos);	
char			*read_word(const char *line, size_t *pos, t_shell *shell);

// Redirect validation functions
int				validate_special_chars(const char *line, size_t *pos);
int				validate_redirect_missing_arg(const char *line, size_t *pos);

// Heredoc functions
t_list			*handle_heredoc(t_lexical_token *tok, t_shell *sh);

// Debugging functions
void			debug_print_token_list(t_list *list);

#endif