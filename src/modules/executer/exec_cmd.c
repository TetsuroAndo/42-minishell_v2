/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:43:40 by teando            #+#    #+#             */
/*   Updated: 2025/04/28 22:44:15 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod_exec.h"

static int	prepare_cmd_args(t_ast *node, char ***argv, int *flag, t_shell *sh)
{
	struct stat	sb;
	t_status status;

	*flag = 0;
	status = handle_redr(node->args, sh);
	if (status)
		return (status);
	*argv = toklist_to_argv(node->args->argv, sh);
	if (!*argv)
	{
		*flag = 1;
		return (E_SYSTEM);
	}
	if (!(*argv)[0])
	{
		*flag = 1;
		return (E_NONE);
	}
	return (E_NONE);
	if (stat((*argv)[0], &sb) == 0 && S_ISDIR(sb.st_mode))
		return (E_IS_DIR);
	return (0);
}

static void	setup_redirections(t_ast *node, t_fdbackup *bk_in,
		t_fdbackup *bk_out, t_shell *sh)
{
	*bk_in = (t_fdbackup){-1, STDIN_FILENO};
	*bk_out = (t_fdbackup){-1, STDOUT_FILENO};
	if (node->args->fds[0] != -1)
	{
		fdbackup_enter(bk_in, STDIN_FILENO, sh);
		xdup2(&node->args->fds[0], STDIN_FILENO, sh);
	}
	if (node->args->fds[1] != -1)
	{
		fdbackup_enter(bk_out, STDOUT_FILENO, sh);
		xdup2(&node->args->fds[1], STDOUT_FILENO, sh);
	}
}

static int	execute_external_cmd(char **argv, t_ast *node, t_shell *sh)
{
	int		wstatus;
	int		sig_held;
	char	**env;

	sig_held = 0;
	sig_ignore_parent(&sig_held);
	node->args->pid = xfork(sh);
	if (node->args->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		env = ft_list_to_strs(sh->env_map);
		if (!*env)
			return (ft_strs_clear(env), E_SYSTEM);
		if (!(*env)[0])
			return (ft_strs_clear(env), E_NONE);
		execve(argv[0], argv, env);
		perror(argv[0]);
		exit(127);
	}
	waitpid(node->args->pid, &wstatus, 0);
	sig_ignore_parent(&sig_held);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (128 + WTERMSIG(wstatus));
}

int	exe_cmd(t_ast *node, t_shell *sh)
{
	char		**argv;
	t_fdbackup	bk_in;
	t_fdbackup	bk_out;
	int			status;
	int			flag;

	flag = 0;
	argv = NULL;
	if (!node || node->ntype != NT_CMD)
		return (1);
	status = ms_lstiter(node->args->argv, proc_exec_path, sh);
	if (status)
		return (status);
	if (sh->debug & DEBUG_SEM)
		debug_print_sem(node, sh);
	status = prepare_cmd_args(node, &argv, &flag, sh);
	if (flag)
		return (status);
	setup_redirections(node, &bk_in, &bk_out, sh);
	if (is_builtin(argv[0]))
		status = builtin_launch(argv, sh);
	else
		status = execute_external_cmd(argv, node, sh);
	fdbackup_exit(&bk_in);
	fdbackup_exit(&bk_out);
	return (status);
}
