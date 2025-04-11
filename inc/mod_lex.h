#ifndef MOD_LEX_H
#define MOD_LEX_H

#include "core.h"

t_status mod_lex(t_shell *shell);

// Tokenization functions
int tokenize_line(t_shell *shell);
t_lexical_token *create_token(t_token_type type, char *value, t_shell *shell);
int add_token(t_shell *shell, t_lexical_token *new_token);
t_token_type get_one_char_op(char c);
t_token_type get_two_char_op(const char *s, size_t *len);
char *read_word(const char *line, size_t *pos, t_shell *shell);

// Debugging functions
void debug_print_token_list(t_list *list);

#endif