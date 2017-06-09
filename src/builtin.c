/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 16:58:48 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/08 18:42:00 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	exec_exit(char **cstin, t_data *data, char *stin)
{
	int s;

	if (cstin[1])
	{
		if (cstin[2])
		{
			ft_putstr_fd("exit: Too many arguments\n", 2);
			data->rvalue = 0;
			return (1);
		}
		else if (!ft_isdigit(cstin[1][0]))
		{
			ft_putstr_fd("exit: Syntax\n", 2);
			data->rvalue = 1;
		}
		else
			data->rvalue = ft_atoi(cstin[1]);
	}
	ft_strdel(&stin);
	s = data->rvalue;
	/*	
		free(data);
		while (data->hist->next)
		data->hist = data->hist->next;
		while (data->hist->before)
		{
		data->hist = data->hist->before;
		ft_strdel(&data->hist->next->elem);
		if (data->hist->next)
		free(data->hist->next);
		}
		if (data->hist)
		{	
		ft_strdel(&data->hist->elem);
		free(data->hist);
		}*/
	exit(s);
	return (1);
}

static void	make_unset(char **cstin, t_env **env)
{
	int i;

	i = 1;
	if (cstin[i])
		while (cstin[i])
			delete_env(env, cstin[i++]);
	else
		ft_putstr_fd("unsetenv: Too few arguments\n", 2);
}

static int	exec_env(char **cstin, t_env **env)
{
	if (cstin[0] && ft_strcmp(cstin[0], "setenv") == 0)
	{
		if (cstin[1] && cstin[2] && cstin[3])
			ft_putstr_fd("setenv: Too many arguments\n", 2);
		else if (cstin[1] && ft_strchr(cstin[1], '='))
			ft_putstr_fd("setenv : '=' not accepted in name variable\n", 2);
		else if (cstin[1] && *env)
			add_env(*env, &cstin[1]);
		else if (cstin[1] && cstin[2])
			addtmp_env(cstin, join(ft_strdup(cstin[1]), "=",
						ft_strdup(cstin[2])), env, 1);
		else if (cstin[1])
			addtmp_env(cstin, join(ft_strdup(cstin[1]), "=",
						ft_strnew(1)), env, 1);
		else
			return (0);
	}
	else if (cstin[0] && ft_strcmp(cstin[0], "unsetenv") == 0)
		make_unset(cstin, env);
	return (1);
}

static int	exec_history(t_data *data)
{
	t_hist	*cursor;
	int		i;

	i = 0;
	cursor = data->hist;
	while (cursor->before)
		cursor = cursor->before;
	while (cursor)
	{
		ft_printf("[%d] %s\n", i++, cursor->elem);
		cursor = cursor->next;
	}
	return (1);
}

static int	exec_help()
{
	ft_putstr("21sh\nKEYBINDING\nLeave current shell [CTRL+D]\n");
	ft_putstr("Clear shell [CTRL+L]\n\nEnter in cpy/cut mode [CTRL+K]\n");
	ft_putstr("Cut selection [CTRL+K]\nCopy selection [CTRL+U]");
	ft_putstr("\nLeave mode [ESC]\nPaste [CTRL+P]\n\n");
	ft_putstr("Move by word left [CTRL+F]\nMove by word right [CTRL+B]\n");
	return (1);
}

int			builtin(char **cstin, t_env **env, char *stin, t_data *data)
{
	if (cstin[0] && (ft_strcmp(cstin[0], "exit") == 0))
	{
		return (exec_exit(cstin, data, stin));
	}
	else if (cstin[0] && (ft_strcmp(cstin[0], "cd") == 0))
		return (exec_cd(cstin, *env));
	else if (cstin[0] && (ft_strcmp(cstin[0], "echo") == 0))
		return (exec_echo(cstin));
	else if (cstin[0] && ((ft_strcmp(cstin[0], "setenv") == 0) ||
				ft_strcmp(cstin[0], "unsetenv") == 0))
		return (exec_env(cstin, env));
	else if (cstin[0] && (ft_strcmp(cstin[0], "history") == 0))
		return (exec_history(data));
	else if (cstin[0] && (ft_strcmp(cstin[0], "help") == 0))
		return (exec_help());
	return (0);
}
