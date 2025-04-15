#ifndef MOD_SYN_H
# define MOD_SYN_H

# include "core.h"

typedef struct s_ast	t_ast;

t_status	mod_syn(t_shell *shell);

void	debug_print_ast(t_ast *ast, t_shell *shell);

#endif
