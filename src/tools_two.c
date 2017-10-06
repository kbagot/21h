/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/03 16:56:01 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/06 22:19:23 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		save_fd(t_data *data)
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

void		destroy_env(t_env **env)
{
	t_env *search;

	if (env)
		while (*env)
		{
			ft_strdel(&(*env)->name);
			ft_strdel(&(*env)->value);
			search = (*env)->next;
			free(*env);
			*env = NULL;
			*env = search;
		}
}

extern int	g_signo;

void		kill_procs(int sig)
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

int			get_proc(int sign)
{
	static int val = 2;

	if (sign != 0)
		val = sign;
	return (val);
}
