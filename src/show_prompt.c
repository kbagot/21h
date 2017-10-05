/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_prompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 17:23:22 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 14:44:38 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int g_signo = 0;

static	void prompt(int sig)
{
	char buf[2];

	if (sig == SIGINT)
	{
		get_proc(1);
		buf[0] = 10;
		buf[1] = 0;
		ioctl(0, TIOCSTI, buf);
	}
}

static void nkill_procs(int sig)
{
	if (sig == SIGINT)
	{
		if (g_signo > 0)
			kill(g_signo, 9);
	}
}

static void		set(char **str, t_env *env)
{
	t_env	*search;
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i][0] == '$' && str[i][1])
		{
			search = search_env(env, &str[i][1]);
			if (search)
			{
				ft_strdel(&str[i]);
				str[i] = ft_strdup(search->value);
			}
			else
			{
				ft_strdel(&str[i]);
				str[i] = ft_strnew(1);
			}
		}
		i++;
	}
}


static void	one_by_one(char **septin, t_data *data, t_env *s_env)
{
	char	**cstin;
	int		i;

	i = 0;
	while (septin[i])
	{
		cstin = splitforquote(septin[i], " \t\n");
		if (cstin && cstin[0])
		{
			save_fd(data);
			creat_heredoc(data, cstin);
			set(cstin, s_env);
			parse_entry(&s_env, cstin, septin[i], data);
		}
		ft_tabdel(&cstin);
		i++;
	}
}

static void printf_prompt(t_env *search, t_env *s_env)
{
	if ((search = search_env(s_env, "PWD")) &&
			ft_strchr(search->value, '/'))
		ft_printf("\033[0;36m[%s]> \033[0m",
				&(ft_strrchr(search->value, '/')[1]));
	else
		ft_printf("\033[0;36m[]> \033[0m");
}

void		show_prompt(t_env *s_env, t_data *data)
{
	char	*stin;
	t_env	*search;
	char	**septin;

	search = NULL;
	stin = NULL;
	while (42)
	{
		printf_prompt(search, s_env);
		signal(SIGINT, prompt);
		stin = line_edit(data);
		signal(SIGINT, kill_procs);
		septin = strquotesplit(stin, ";");//use stin fot history
		if (parse_error(septin) && stin) // token
			one_by_one(septin, data, s_env);
		signal(SIGINT, nkill_procs);
		kill(0, SIGINT);
		ft_tabdel(&septin);
		ft_strdel(&stin);
	}
}
