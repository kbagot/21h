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
