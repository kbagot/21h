/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 16:58:48 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/03 16:50:01 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

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

static int	exec_help(void)
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
