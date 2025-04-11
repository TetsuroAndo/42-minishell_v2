/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:45:42 by teando            #+#    #+#             */
/*   Updated: 2025/04/11 21:01:59 by teando           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "mod_syn.h"

t_args	*args_new(t_shell *shell)
{
	t_args	*args;

	args = (t_args *)xmalloc(sizeof(t_args), shell);
	args->path[0] = '\0';
	args->cargv = NULL;
	args->argv = NULL;
	args->redr = NULL;
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

static void	*ms_listshift(t_list *list)
{
	t_list	*tmp;

	if (!list)
		return (NULL);
	tmp = list;
	list = list->next;
	return (tmp);
}

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
void	ast_redir(t_ast *node, t_shell *shell)
{
	t_lexical_token	*token;

	token = (t_lexical_token *)shell->token_list->data;
	if ((token->value == NULL))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n",
			redir_token_to_symbol(token->type));
		shell_exit(shell, E_SYNTAX);
	}
	if (!node->args->redr)
		node->args->redr = xlstnew(ms_listshift(shell->token_list), shell);
	else
		ft_lstadd_back(&node->args->redr,
			xlstnew(ms_listshift(shell->token_list), shell));
	return (node);
}

/*
複数のリダイレクトをしまう
redir redirections*
*/
t_ast	*ast_redirections(t_ast *node, t_shell *shell)
{
	t_lexical_token	*token;

	if (!node)
	{
		node = ast_new(NT_REDIRECT, NULL, NULL, shell);
		node->args = args_new(shell);
	}
	ast_redir(node, shell);
	token = (t_lexical_token *)shell->token_list->data;
	if ((token->type & 0xFF00) == TM_REDIR)
		ast_redirections(node, shell);
	return (node);
}

/*コマンドとオプションにするところ*/
/* WORD (WORD)* */
t_ast	*ast_simple_cmd(t_shell *shell)
{
	t_ast	*node;

	node = ast_new(NT_CMD, NULL, NULL, shell);
	return (node);
}

/*
コマンドとリダイレクト
simple_cmd redirections?
*/
t_ast	*ast_cmd(t_shell *shell)
{
	t_ast	*node;

	return (node);
}

/*
コマンド　or カッコの処理
command | '(' list ')'
*/
t_ast	*ast_primary(t_shell *shell)
{
	t_ast	*node;

	return (node);
}

/*
複数のコマンドをつなげる（一個のときにある）
primary ( '|' primary )*
*/
t_ast	*ast_pipeline(t_shell *shell)
{
	t_ast	*node;

	return (node);
}

/*

pipeline	(('&&' | '||') pipeline )
*/
t_ast	*ast_and_or(void)
{
	t_ast	*node;

	return (node);
}

/*

and_or ( ';' and_or)*
*/
t_ast	*ast_list(t_list token, t_shell *shell)
{
	t_ast	*node;

	return (node);
}

/* list */
t_status	mod_syn(t_shell *shell)
{
	t_list	*tok_head;
	t_ast	*ast;

	tok_head = shell->token_list;
	ast = pipe_list(tok_head, shell);
	while (1)
	{
		if (pipe_list(tok_head, shell) == E_NONE)
			;
		else
			;
		break ;
	}
	return (E_NONE);
}
