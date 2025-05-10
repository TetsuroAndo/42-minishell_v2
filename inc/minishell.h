/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 03:24:51 by teando            #+#    #+#             */
/*   Updated: 2025/05/10 15:08:48 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "core.h"
# include "mod_exec.h"
# include "mod_lex.h"
# include "mod_sem.h"
# include "mod_syn.h"
# include "ms_readline.h"
# include "ms_signal.h"

# ifndef PROMPT
#  define PROMPT "minishell$ "
# endif

#endif
