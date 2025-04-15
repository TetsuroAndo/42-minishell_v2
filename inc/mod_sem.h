#ifndef MOD_SEM_H
# define MOD_SEM_H

# include "core.h"

typedef struct s_lexical_token	t_lexical_token;
typedef struct s_args			t_args;
typedef struct s_ast			t_ast;

typedef enum e_sem_trigger
{
	ST_NONE,       // 参照なし
	ST_CMD,        // BILDIN or PATH
	ST_QUOTE,      // "'`
	ST_BACK_SLASH, // バックスラッシュ
	ST_WILDCARD,   // *
	ST_DOLLAR,     // $VAR $?
	ST_REDIRECT    // < > << >>
}								t_sem_trigger;

typedef enum e_quote
{
	QS_QUOTE_NONE,   // クォートなし
	QS_QUOTE_SINGLE, // シングルクォート内
	QS_QUOTE_DOUBLE, // ダブルクォート内
	QS_QUOTE_BACK,   // バッククォート内
}								t_quote;

#endif
