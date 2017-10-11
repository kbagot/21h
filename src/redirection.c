/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 21:02:13 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/10 17:29:10 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	redir_output(char **cmd, t_data *d, int i)
{
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 1;
	if ((d->out = open(cmd[i], O_CREAT | O_WRONLY | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) != -1)
		dup2(d->out, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->out >= 0 && d->out <= 2))
		close(d->out);
}

static void	redir_input(char **cmd, t_data *d, int i)
{
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 0;
	if ((d->in = open(cmd[i], O_RDONLY)) != -1)
		dup2(d->in, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->in >= 0 && d->in <= 2))
		close(d->in);
}

static void	append_redir_output(char **cmd, t_data *d, int i)
{
	int up;

	if (i != 1)
		up = ft_atoi(cmd[0]);
	else
		up = 1;
	if ((d->out = open(cmd[i], O_CREAT | O_WRONLY | O_APPEND,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) != -1)
		dup2(d->out, up);
	else
		ft_putstr_fd("21sh: permission denied\n", 2);
	if (!(d->in >= 0 && d->in <= 2))
		close(d->out);
}

static int	exec_redir(char **cmd, int i, t_data *d)
{
	if (i >= 0 && !ft_strcmp(cmd[i - 1], ">"))
		redir_output(cmd, d, i);
	else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<"))
		redir_input(cmd, d, i);
	else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">>"))
		append_redir_output(cmd, d, i);
	else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">&"))
	{
		if (dup_in_out_put(cmd, i, STDOUT_FILENO))
			return (1);
	}
	else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<&"))
		if (dup_in_out_put(cmd, i, STDIN_FILENO))
			return (1);
	return (0);
}

int			make_redir(char **rdr, t_data *d)
{
	char	**cmd;
	int		i;
	int		j;

	j = 0;
	cmd = NULL;
	while (rdr[j])
	{
		i = 0;
		cmd = ft_strsplit(rdr[j], ' ');
		while (cmd[i])
			i++;
		i--;
		if (exec_redir(cmd, i, d))
		{
			ft_tabdel(&cmd);
			return (1);
		}
		j++;
		ft_tabdel(&cmd);
	}
	return (0);
}
