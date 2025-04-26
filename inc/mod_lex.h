#ifndef MOD_LEX_H
# define MOD_LEX_H

# include "core.h"

t_status		mod_lex(t_shell *shell);

// Tokenization functions
int				add_token(t_shell *shell, t_lexical_token *new_token);
t_lexical_token	*create_token(t_token_type type, char *value, t_shell *shell);
int				tokenize_line(t_shell *shell);
t_token_type	get_one_char_op(int c);
t_token_type	get_two_char_op(const char *s, size_t *len);
char			*read_word(const char *line, size_t *pos, t_shell *shell);

// Redirect validation functions
int				validate_special_chars(const char *line, size_t *pos);
int				validate_redirect_missing_arg(const char *line, size_t *pos);

// Heredoc functions
t_list			*handle_heredoc(t_lexical_token *tok, t_shell *sh);

// Debugging functions
void			debug_print_token_list(t_list *list);

#endif