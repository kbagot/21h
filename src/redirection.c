/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 21:02:13 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 21:06:18 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	redir_output(char **cmd, t_data *d, int i)
{//  >
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
{// <
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
{// >>
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

static int	dup_output(char **cmd, int i)
{
	int n;
	int w;

	//printf("[%s][%s][%s] %d \n", cmd[0], cmd[1], cmd[2], i);
	if (i != STDOUT_FILENO)
		n = ft_atoi(cmd[0]);
	else
		n = STDOUT_FILENO;
	if (!ft_strcmp(cmd[i], "-"))
		close (n);
	if (ft_isnbr(cmd[i]))
	{
		int t;
		t = -1;
		//ft_printf("%d\n", d->out);
		w = ft_atoi(cmd[i]);
		//		printf("%d \n", d->out);
		if ((w >= 10 && w <= 12) || (t = dup(w)) == -1)
		{
			//		printf("%d \n", t);
			//		printf("%d \n", w);
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(w, 2);
			ft_putstr_fd(": Bad file descriptor\n", 2);
			return (1);
		}
		if (t != -1)
		{
			//		printf("%ddelt \n", t);
			close (t);
		}
		dup2(w, n);
	}
	return (0);
}

static int	dup_input(char **cmd, int i)
{ //mirror of upper fct 
	int n;
	int w;

	//printf("[%s][%s][%s] %d \n", cmd[0], cmd[1], cmd[2], i);
	if (i != STDIN_FILENO)
		n = ft_atoi(cmd[0]);
	else
		n = STDIN_FILENO;
	if (!ft_strcmp(cmd[i], "-"))
		close (n);
	if (ft_isnbr(cmd[i]))
	{
		int t;
		t = -1;
		//ft_printf("%d\n", d->out);
		w = ft_atoi(cmd[i]);
		//		printf("%d \n", d->out);
		if ((w >= 10 && w <= 12) || (t = dup(w)) == -1)
		{
			//		printf("%d \n", t);
			//		printf("%d \n", w);
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(w, 2);
			ft_putstr_fd(": Bad file descriptor\n", 2);
			return (1);
		}
		if (t != -1)
		{
			//		printf("%ddelt \n", t);
			close(t);
		}
		dup2(w, n);
	}
	return (0);
}

int exec_redir(char **rdr, t_data *d)
{
	char	**cmd;
	int		i;
	int		j;

	j = 0;
	i = 0;
	cmd = NULL;
	while (rdr[j])
	{
		cmd = ft_strsplit(rdr[j], ' ');
		while (cmd[i])
			i++;
		i--;
		if (i >= 0 && !ft_strcmp(cmd[i - 1], ">"))
			redir_output(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<"))
			redir_input(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">>"))
			append_redir_output(cmd, d, i);
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], ">&"))
		{
			if (dup_output(cmd, i))
				return (1);
		}
		else if (i >= 0 && !ft_strcmp(cmd[i - 1], "<&"))
		{
			if (dup_input(cmd, i))
				return (1);
		}
		i = 0;
		j++;
	}
	ft_tabdel(&cmd);
	return (0);
}
