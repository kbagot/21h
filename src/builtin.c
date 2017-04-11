/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 16:58:48 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/07 12:32:55 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	free(data);
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

int			builtin(char **cstin, t_env **env, char *stin, t_data *data)
{
	if (cstin[0] && (ft_strcmp(cstin[0], "exit") == 0))
	{
		return (exec_exit(cstin, data, stin));
	}
	else if (cstin[0] && (ft_strcmp(cstin[0], "cd") == 0))
		return (exec_cd(cstin, *env));
	else if (cstin[0] && (ft_strcmp(cstin[0], "echo") == 0 &&
				ft_strlen(stin) > 5))
		return (exec_echo(&stin[5]));
	else if (cstin[0] && ((ft_strcmp(cstin[0], "setenv") == 0) ||
				ft_strcmp(cstin[0], "unsetenv") == 0))
		return (exec_env(cstin, env));
	return (0);
}
