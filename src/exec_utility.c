/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:58:40 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 21:10:41 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern int g_signo;

static int	invalid_exec(char **stin, char **env, t_data *data)
{
	if ((stin[0] && (access(stin[0], X_OK) == -1)) || !stin[0])
	{
		ft_tabdel(&env);
		if (stin[0])
		{
			ft_putstr_fd("21sh: command not found: ", 2);
			ft_putstr_fd(stin[0], 2);
			ft_putchar_fd('\n', 2);
			data->rvalue = 1;
		}
		return (1);
	}
	return (0);
}

static void	sig(int rvalue, pid_t pid, char *stin)
{
	if (WTERMSIG(rvalue) == SIGSEGV)
	{
		ft_putnbr_fd(pid, 2);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd("segmentation fault", 2);
		ft_putstr_fd("  ", 2);
		ft_putstr_fd(stin, 2);
		ft_putchar_fd('\n', 2);
	}
	else if (WTERMSIG(rvalue) == SIGBUS)
	{
		ft_putnbr_fd(pid, 2);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd("bus error", 2);
		ft_putstr_fd("  ", 2);
		ft_putstr_fd(stin, 2);
		ft_putchar_fd('\n', 2);
	}
}

static void	exec_utility(char **env, char **stin, t_data *data)
{
	pid_t	pid;
	int		rvalue;

	rvalue = 0;
	if (invalid_exec(stin, env, data))
		return ;
	if ((pid = fork()) == 0)
	{
		if (execve(stin[0], stin, env) == -1)
		{
			ft_putstr_fd("Exec format error\n", 2);
			_exit(1);
		}
	}
	else if (pid > 0)
	{
		g_signo = pid;
		wait(&rvalue);
		if (data->lastpid != 0)
			kill(data->lastpid, SIGTERM);
		if (WIFEXITED(rvalue))
			data->rvalue = WEXITSTATUS(rvalue);
		else if (WIFSIGNALED(rvalue))
			sig(rvalue, pid, stin[0]);
	}
	ft_tabdel(&env);
}

static t_line	*fork_pipes(t_line *line, t_data *d)
{
	t_line *old;
	pid_t	pid;
	int		fd[2];

	pid = 0;
	if (line)
	{
		while (line->next)
		{
			pipe(fd);
			d->out = fd[1];
			if ((pid = fork()) == 0)
			{
				if (d->in != 0)
				{
					dup2(d->in, 0);
					close(d->in);
				}
				if (d->out != 1)
				{
					dup2(d->out, 1);
					close(d->out);
				}
				return (line);
			}
			d->lastpid = pid;
			close(d->out);
			d->in = fd[0];
			old = line;
			line = line->next;
			ft_tabdel(&old->proc);
			ft_tabdel(&old->redirect);
			ft_memdel((void**)&old);
		}
		if (d->in != 0)
			dup2(d->in, 0);
	}
	return (line);
}

void kill_procs(int sig)
{
	if (sig == SIGINT)
	{
		if (g_signo > 0)
		{
			kill(g_signo, 9);
			ft_putchar('\n');
		}
	}
}

int		get_proc(int sign)
{
	static int val = 2;

	if (sign != 0)
		val = sign;
	return (val);
}

static void reset_fd(t_data *data)
{
	dup2(data->stdin_cpy, 0);
	close(data->stdin_cpy);
	dup2(data->stdout_cpy, 1);
	close(data->stdout_cpy);
	dup2(data->stderr_cpy, 2);
	close(data->stderr_cpy);
	//	ft_printf("%d %d %d \n", data->in , data->out, data->err); Should keep it ?
	//	if (data->in != 0)
	//		close (data->in);
	//	if (data->out != 1)
	//		close (data->out);
	//	if (data->err != 2)
	//		close (data->err);
}

void	save_fd(t_data *data)
{
	data->stdin_cpy = 10;
	data->stdout_cpy = 11;
	data->stderr_cpy = 12;
	dup2(STDIN_FILENO, 10);
	dup2(STDOUT_FILENO, 11);
	dup2(STDERR_FILENO, 12);
	data->in = STDIN_FILENO;
	data->out = STDOUT_FILENO;
	data->err = STDERR_FILENO;
}

static int enter_builtin(t_data *data, t_env **s_env, char *stin, t_line *line)
{
	if ((data->rvalue = exec_redir(line->redirect, data)) ||
			(data->rvalue = builtin(line->proc, s_env, stin, data)))
	{
		if (line->next)
			_exit(data->rvalue);
		else
		{
			reset_fd(data);
			return (1);
		}
	}
	return (0);
}

static void	enter_utility(t_data *data, t_env **s_env, t_line *line)
{
	t_env	*tmp_env;

	tmp_env = NULL;
	tmp_env = master_env(*s_env, line->proc, tmp_env);
	if ((line->proc = utility(line->proc, *s_env)))
		exec_utility(list_to_tab(tmp_env), line->proc, data);
	if (line->next)
		_exit(data->rvalue);
	destroy_env(&tmp_env);
}

void		parse_entry(t_env **s_env, char **cstin, char *stin, t_data *data)
{
	t_line	*line;
	int status;
	int pid;

	line = split_pipe(cstin);
	line = fork_pipes(line, data);
	if (!enter_builtin(data, s_env, stin, line))
		enter_utility(data, s_env, line);
	else
	{
	while ((pid = wait(&status)) > 0)
		status = 0;
	reset_fd(data);
		ft_tabdel(&line->proc);
		ft_tabdel(&line->redirect);
		ft_memdel((void**)&line);
		return ;
	}
	while ((pid = wait(&status)) > 0)
		status = 0;
	reset_fd(data);
	ft_tabdel(&line->proc);
	ft_tabdel(&line->redirect);
	ft_memdel((void**)&line);
}
