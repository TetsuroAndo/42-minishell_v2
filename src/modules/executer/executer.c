/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:40:08 by tomsato           #+#    #+#             */
/*   Updated: 2025/04/18 21:43:02 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

t_status	mod_exec(t_shell *shell)
{
	t_ast *ast;

	ast = shell->ast;
	return (shell->status);
}
