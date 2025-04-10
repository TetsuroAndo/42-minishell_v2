#ifndef MOD_LEX_H
#define MOD_LEX_H

#include "core.h"

// Lexical analysis functions
t_status launch_lexer(t_shell *shell);
int tokenize_line(t_shell *shell);
t_lexical_token *create_token(t_token_type type, char *value, t_shell *shell);
int add_token(t_shell *shell, t_lexical_token *new_token);
t_token_type get_one_char_op(char c);
t_token_type get_two_char_op(const char *s, size_t *len);
char *read_word(const char *line, size_t *pos, t_shell *shell);
void debug_print_token_list(t_list *list);
const char *type_to_str(t_token_type t);

#endif