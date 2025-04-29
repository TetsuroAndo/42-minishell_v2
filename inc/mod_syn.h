/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_syn.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:20:00 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:08:57 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOD_SYN_H
# define MOD_SYN_H

# include "core.h"

typedef struct s_pl
{
	t_list			*lst;
}					t_pl;

typedef struct s_binop
{
	t_token_type	tt;
	t_ntype			ntype;
}					t_binop;

t_status			mod_syn(t_shell *shell);
void				debug_print_ast(t_ast *ast, t_shell *shell);

int					tok_is_redir(t_token_type t);
int					tok_is_eof(t_token_type t);
t_lexical_token		*tok_peek(t_pl *pl);
t_lexical_token		*tok_pop_dup(t_pl *pl, t_shell *sh);
t_status			synerr(t_shell *sh, const char *msg);

t_args				*args_new(t_shell *sh);
t_ast				*ast_make(t_ntype type, t_ast *l, t_ast *r, t_shell *sh);

t_ast				*parse_primary(t_pl *pl, t_shell *sh);
t_ast				*parse_pipeline(t_pl *pl, t_shell *sh);
t_ast				*parse_and_or(t_pl *pl, t_shell *sh);
t_ast				*parse_list(t_pl *pl, t_shell *sh);

#endif
