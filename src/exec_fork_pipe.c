/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/05 15:09:53 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 15:40:07 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	next_pipe(pid_t pid, t_data *d, t_line **line, int fd[2])
{
	t_line *old;

	d->lastpid = pid;
	close(d->out);
	d->in = fd[0];
	old = *line;
	*line = (*line)->next;
	ft_tabdel(&old->proc);
	ft_tabdel(&old->redirect);
	ft_memdel((void**)&old);
}

static void	link_fd(int old, int new)
{
	if (old != new)
	{
		dup2(old, new);
		close(old);
	}
}

t_line		*fork_pipes(t_line *line, t_data *d)
{
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
				link_fd(d->in, 0);
				link_fd(d->out, 1);
				return (line);
			}
			next_pipe(pid, d, &line, fd);
		}
		if (d->in != 0)
			dup2(d->in, 0);
	}
	return (line);
}
