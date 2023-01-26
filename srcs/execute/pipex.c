/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcollon <jcollon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 01:28:24 by jcollon           #+#    #+#             */
/*   Updated: 2023/01/26 23:59:46 by jcollon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"
#include <sys/wait.h>
#include "minishell.h"

/**
 * @brief Wait for all the pid in the list
 * and close the standard input of the next command
 * 
 * @param pids: List of pid
 * @return The status code of the last command
 */
int	wait_pids(t_fd_lst **pids)
{
	t_fd_lst	*lst;
	int			pid;

	if (!(*pids) || !(*pids)->fd)
		return (0);
	lst = (*pids);
	waitpid(lst->fd, &pid, 0);
	lst = lst->next;
	while (lst)
	{
		waitpid(lst->fd, NULL, 0);
		lst = lst->next;
	}
	return (WEXITSTATUS(pid));
}

/**
 * @brief Launch a pipe and adds the pid to the list
 * 
 * @param pipe: the pipe to launch
 * @param envp: the environment variable
 * @param std_ins: the list of standard input
 * @param pids: the list of pid
 * @return 1 if an error occured, 2 if the command was not found, 3 if it was a 
 * directory, 4 if HOME is not set and was needed, 0 otherwise
 */
char	launch_pipe(t_pipe *pipe, char **envp, t_fd_lst **std_ins,
	t_fd_lst **pids)
{
	int	tmp;

	if (fd_lst_add_front(pids, fd_lst_new(launch_prog(pipe, envp, std_ins)))
		|| (*pids)->fd <= 0)
	{
		if (pids && *pids && ((*pids)->fd == 0 || (*pids)->fd != -256))
		{
			tmp = (*pids)->fd;
			*pids = fd_lst_del_one(*pids);
			if (fd_lst_add_front(std_ins,
					fd_lst_new(open("/dev/null", O_RDONLY))))
				return (1);
			return (cmd_not_found(pipe->args[0], tmp));
		}
		else
		{
			clear_split(envp);
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Launch all the command in the pipe list
 * 
 * @param pipe_lst: the pipe list
 * @param envp: the environment
 * @return int the status code of the last command
 */
int	pipex(t_pipe **pipe_lst, char ***envp)
{
	t_fd_lst	*std_ins;
	t_fd_lst	*pids;
	t_pipe		*pipe;
	char		tmp;
	int			ret;

	ret = is_built_in((*pipe_lst)->args[0]);
	if (ret > 0 && pipe_len(*pipe_lst) == 2)
		return (run_built_in(ret, pipe_lst, envp, 1));
	pids = NULL;
	std_ins = NULL;
	pipe = *pipe_lst;
	while (pipe->next)
	{
		tmp = launch_pipe(pipe, *envp, &std_ins, &pids);
		if (tmp)
			error_execve(tmp, *pipe_lst, std_ins, pids);
		pipe = pipe->next;
	}
	ret = wait_pids(&pids);
	clear_pipe_lst(*pipe_lst, 1);
	free_fds(std_ins, pids);
	return (ret);
}
