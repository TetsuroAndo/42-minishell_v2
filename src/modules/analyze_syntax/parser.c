/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:45:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/27 10:34:56 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_syn.h"

t_ast					*ast_list(t_list **tok_lst, t_shell *shell);
void					free_ast(t_ast **ast);

t_args	*args_new(t_shell *shell)
{
	t_args	*args;

	args = (t_args *)xmalloc(sizeof(t_args), shell);
	args->argv = NULL;
	args->redr = NULL;
	args->b_argv = NULL;
	args->b_redr = NULL;
	args->fds[0] = -1;
	args->fds[1] = -1;
	args->pid = -1;
	return (args);
}

t_ast	*ast_new(t_ntype type, t_ast *left, t_ast *right, t_shell *shell)
{
	t_ast	*node;

	node = (t_ast *)xmalloc(sizeof(t_ast), shell);
	node->left = left;
	node->right = right;
	node->ntype = type;
	return (node);
}

t_lexical_token	*curr_token(t_list **token_list)
{
	if (!token_list || !(*token_list) || !(*token_list)->data)
		return (NULL);
	return ((t_lexical_token *)(*token_list)->data);
}

static t_lexical_token	*alloc_l_tok(t_list **tok_lst, t_shell *sh)
{
	t_lexical_token	*tok;
	t_lexical_token	*res_val;

	tok = curr_token(tok_lst);
	if (!tok)
		return (NULL);
	if ((tok->type & 0xFF00) == TM_REDIR)
		res_val = xmalloc_gcline(sizeof(t_lexical_token), sh);
	else
		res_val = xmalloc(sizeof(t_lexical_token), sh);
	ft_memcpy(res_val, tok, sizeof(t_lexical_token));
	if (tok->value && (tok->type & 0xFF00) == TM_REDIR)
	{
		res_val->value = ms_strdup_gcli(tok->value, sh);
	}
	else if (tok->value)
	{
		res_val->value = ms_strdup(tok->value, sh);
	}
	ms_listshift(tok_lst);
	return (res_val);
}

/*
** ========== Redirection Parsing ==========
**  redir         ::= ( '>' | '<' | '>>' | '<<') WORD
**  redirections  ::= redir redirections?
*/
static const char	*redir_token_to_symbol(t_token_type token_type)
{
	if (token_type == TT_REDIR_IN)
		return ("<");
	else if (token_type == TT_APPEND)
		return (">>");
	else if (token_type == TT_REDIR_OUT)
		return (">");
	else if (token_type == TT_HEREDOC)
		return ("<<");
	else
		return (NULL);
}

/*
リダイレクト（一個）
redir
*/
int	ast_redir(t_list **tok_lst, t_ast *node, t_shell *shell)
{
	t_lexical_token	*new_tok;
	t_list			*new_node;

	if (!curr_token(tok_lst))
		return (1);
	if (*curr_token(tok_lst)->value == '\0')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n",
			redir_token_to_symbol(curr_token(tok_lst)->type));
		return (1);
	}
	new_tok = alloc_l_tok(tok_lst, shell);
	if (!new_tok)
		return (1);
	new_node = xlstnew_gcli(new_tok, shell);
	if (!node->args->redr)
		node->args->redr = new_node;
	else
		ft_lstadd_back(&node->args->redr, new_node);
	return (0);
}

/*
複数のリダイレクトをしまう
redir redirections*
*/
int	ast_redirections(t_list **tok_lst, t_ast *node, t_shell *sh)
{
	t_lexical_token	*tok;

	while ((tok = curr_token(tok_lst)) && (tok->type & 0xFF00) == TM_REDIR)
		if (ast_redir(tok_lst, node, sh)) /* エラー時は 1 を返すだけ */
			return (1);
	return (0);
}

/* コマンドとオプションにするところ
** ========== Command Parsing ==========
**  simple_cmd ::= WORD (WORD)*
**  cmd ::= simple_cmd redirections?
*/
t_ast	*ast_simple_cmd(t_list **tok_lst, t_ast *node, t_shell *shell)
{
	t_lexical_token	*tok;

	tok = curr_token(tok_lst);
	while (tok && tok->type == TT_WORD)
	{
		ft_lstadd_back(&node->args->argv, xlstnew(alloc_l_tok(tok_lst, shell),
				shell));
		tok = curr_token(tok_lst);
	}
	return (node);
}

/*
コマンドとリダイレクト
simple_cmd redirections?
*/
t_ast	*ast_cmd(t_list **tok_lst, t_shell *sh)
{
	t_ast	*node;

	node = ast_new(NT_CMD, NULL, NULL, sh);
	node->args = args_new(sh);
	if (ast_redirections(tok_lst, node, sh))
		return (free_ast(&node), NULL);
	ast_simple_cmd(tok_lst, node, sh);
	if (ast_redirections(tok_lst, node, sh))
		return (free_ast(&node), NULL);
	return (node);
}

/* コマンド or カッコの処理
** ========== Primary (cmd or '(' list ')') ==========
**  primary ::= cmd | '(' list ')'
*/
t_ast	*ast_primary(t_list **tok_lst, t_shell *shell)
{
	t_ast			*node;
	t_lexical_token	*tok;

	if (!shell || !*tok_lst)
		return (NULL);
	tok = curr_token(tok_lst);
	if (!tok)
		return (NULL);
	if (((tok->type & 0xFF00) != TM_REDIR && tok->type != TT_WORD)
		&& tok->type != TT_LPAREN)
		return (NULL);
	if ((tok->type & 0xFF00) == TM_REDIR || tok->type == TT_WORD)
		return (ast_cmd(tok_lst, shell));
	if (tok->type == TT_LPAREN)
	{
		ms_listshift(tok_lst);
		node = ast_new(NT_SUBSHELL, ast_list(tok_lst, shell), NULL, shell);
		tok = curr_token(tok_lst);
		if (tok->type != TT_RPAREN)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token`('\n");
			return (free_ast(&node), NULL);
		}
		ms_listshift(tok_lst);
		return (node);
	}
	return (NULL);
}

/* 複数のコマンドをつなげる（一個のときにある）
** パイプの(前 | 後)に何もなければエラーにする
** ========== Pipeline ==========
**  pipeline ::= primary ( '|' primary )*
*/
t_ast	*ast_pipeline(t_list **tok_lst, t_shell *shell)
{
	t_ast			*node;
	t_lexical_token	*tok;

	node = ast_primary(tok_lst, shell);
	tok = curr_token(tok_lst);
	while (tok && tok->type == TT_PIPE)
	{
		if (node == NULL)
		{
			return (NULL);
		}
		ms_listshift(tok_lst);
		node = ast_new(NT_PIPE, node, ast_primary(tok_lst, shell), shell);
		if (node->right == NULL)
		{
			return (free_ast(&node), NULL);
		}
		tok = curr_token(tok_lst);
	}
	return (node);
}

/*
** ========== And/Or ==========
**  and_or ::= pipeline ( ( '&&' | '||' ) pipeline )*
*/
// filepath:
t_ast	*ast_and_or(t_list **tok_lst, t_shell *shell)
{
	t_ntype			op_type;
	t_lexical_token	*tok;
	t_ast			*left;
	t_ast			*right;

	left = ast_pipeline(tok_lst, shell);
	if (!left)
		return (NULL);
	tok = curr_token(tok_lst);
	while (tok && (tok->type == TT_AND_AND || tok->type == TT_OR_OR))
	{
		if (tok->type == TT_AND_AND)
			op_type = NT_AND;
		else if (tok->type == TT_OR_OR)
			op_type = NT_OR;
		ms_listshift(tok_lst);
		right = ast_pipeline(tok_lst, shell);
		if (!right)
			return (free_ast(&left), NULL);
		left = ast_new(op_type, left, right, shell);
		tok = curr_token(tok_lst);
	}
	return (left);
}

/*
** ========== List (Top Level) ==========
**  list ::= and_or ( ';' and_or )*
*/
t_ast	*ast_list(t_list **tok_lst, t_shell *shell)
{
	t_ast	*node;

	// t_ast			*right;
	// t_lexical_token	*tok;
	node = ast_and_or(tok_lst, shell);
	if (!node)
		return (NULL);
	// tok = curr_token(tok_lst);
	// while (tok && tok->type == TT_SEMICOLON)
	// {
	// 	ms_listshift(tok_lst);
	// 	tok = curr_token(tok_lst);
	// 	if (tok && tok->type == TT_SEMICOLON)
	// 	{
	// 		ft_dprintf(STDERR_FILENO,
	// 			"minishell: syntax error near unexpected token `;;'\n");
	// 		return (free_ast(&node), NULL);
	// 	}
	// 	if (!tok || tok->type == TT_EOF)
	// 		break ;
	// 	right = ast_and_or(tok_lst, shell);
	// 	if (!right)
	// 		return (free_ast(&node), NULL);
	// 	node = ast_new(NT_LIST, node, right, shell);
	// 	tok = curr_token(tok_lst);
	// }
	return (node);
}

const char	*tt_to_symbol(t_lexical_token token)
{
	if (token.type == TT_REDIR_IN)
		return (" `<'");
	if (token.type == TT_APPEND)
		return (" `>>'");
	if (token.type == TT_REDIR_OUT)
		return (" `>'");
	if (token.type == TT_HEREDOC)
		return (" `<<'");
	if (token.type == TT_LPAREN)
		return (" `('");
	if (token.type == TT_RPAREN)
		return (" `)'");
	if (token.type == TT_AND_AND)
		return (" `&&'");
	if (token.type == TT_OR_OR)
		return (" `||'");
	if (token.type == TT_SEMICOLON)
		return (" `;'");
	return ("");
}
/*
** ========== Entry Point ==========
**  mod_syn ::= list
*/
t_status	mod_syn(t_shell *shell)
{
	t_ast			*ast;
	t_list			**tok_head;
	t_lexical_token	*tok;

	shell->token_list_syn = shell->token_list;
	tok_head = &shell->token_list_syn;
	shell->ast = NULL;
	ast = ast_list(tok_head, shell);
	tok = curr_token(tok_head);
	if (!ast || (tok && tok->type != TT_EOF))
	{
		if (tok->type == TT_EOF)
			return (E_NONE);
		if ((tok->type & 0xFF00) != TM_REDIR)
			ft_dprintf(STDERR_FILENO, ES_TOKEN, tt_to_symbol(*tok));
		shell->status = E_SYNTAX;
		return (free_ast(&ast), E_SYNTAX);
	}
	shell->ast = ast;
	if (shell->debug & DEBUG_SYN)
		debug_print_ast(ast, shell);
	return (E_NONE);
}
