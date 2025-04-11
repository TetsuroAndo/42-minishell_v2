#ifndef MS_TOKEN_H
#define MS_TOKEN_H

#include "libft.h"
#include <limits.h>

typedef enum e_token_type
{
    /* Command and Arguments */
    TT_WORD = 0x1101,

    /* Pipe */
    TT_PIPE = 0x302,

    /* Redirections */
    TT_REDIR_IN = 0x1201,
    TT_APPEND = 0x1202,
    TT_REDIR_OUT = 0x1203,
    TT_HEREDOC = 0x1204,

    /* Parentheses */
    TT_LPAREN = 0x501,
    TT_RPAREN = 0x502,

    /* Control Operators */
    TT_AND_AND = 0x2001,
    TT_OR_OR = 0x2002,
    TT_SEMICOLON = 0x2003,

    /* Special Tokens */
    TT_EOF = 0x2004,
    TT_ERROR = 0x002
} t_token_type;

typedef struct s_lexical_token
{
    t_token_type type;
    char *value;
} t_lexical_token;

#endif
