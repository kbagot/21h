/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 14:36:14 by kbagot            #+#    #+#             */
/*   Updated: 2017/05/05 18:31:19 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		ms_dat(t_env *new_env, char **env, int i, int j)
{
	if ((ft_strcmp(new_env->name, "SHELL")) == 0)
		new_env->value = ft_strdup("~/Documents/Projet/minishell/minishell");
	else
		new_env->value = ft_strsub(env[i], j + 1, ft_strlen(env[i]) - j);
}

t_env			*env_build(char **env, t_env *first_env)
{
	t_env	*bef_env;
	t_env	*new_env;
	int		i;
	int		j;

	j = 0;
	i = 0;
	bef_env = NULL;
	while (env[i])
	{
		new_env = (t_env*)malloc(sizeof(t_env));
		if (i == 0)
			first_env = new_env;
		if (bef_env)
			bef_env->next = new_env;
		while (env[i][j] && env[i][j] != '=')
			j++;
		new_env->name = ft_strsub(env[i], 0, j);
		ms_dat(new_env, env, i, j);
		j = 0;
		bef_env = new_env;
		new_env->next = NULL;
		i++;
	}
	return (first_env);
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

static int	parse_error(char **stin)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (stin && stin[i])
	{
		tmp = strmsplit(stin[i], " \t\n");
		j = 0;
		while (tmp[j])
			j++;
		if (ft_strcmp(tmp[0], "|") == 0 || (j > 0 &&
					ft_strcmp(tmp[j - 1], "|") == 0))
		{
			ft_tabdel(&tmp);
			ft_putstr_fd("21sh: parse error near `|'\n", 2);
			return (0);
		}
		ft_tabdel(&tmp);
		i++;
	}
	return (1);
}

void		show_prompt(t_env *s_env, t_data *data)
{
	char	*stin;
	char	**cstin;
	t_env	*search;
	char	**septin;
	int		i;

	stin = NULL;
	while (42)
	{
		i = 0;
		if ((search = search_env(s_env, "PWD")) &&
				ft_strchr(search->value, '/'))
			ft_printf("\033[0;36m[%s]> \033[0m",
					&(ft_strrchr(search->value, '/')[1]));
		else
			ft_printf("\033[0;36m[]> \033[0m");
		stin = line_edit(data);
		septin = ft_strsplit(stin, ';');//use stin fot history
		if (parse_error(septin))
		{
			while (stin && septin[i])
			{
				cstin = strmsplit(septin[i], " \t\n");
				set(cstin, s_env);
				parse_entry(&s_env, cstin, septin[i], data);
				ft_tabdel(&cstin);
				i++;
			}
		}
		ft_tabdel(&septin);
		ft_strdel(&stin);
	}
}

int				main(int ac, char **av, char **env)
{
	t_env	*s_env;
	char	*tmp;
	t_env	*search;
	t_data	*data;

	data = ft_memalloc(sizeof(t_data));
	data->rvalue = 0;
	data->hist = NULL;
	if (ac == 1)
	{
		av = NULL;
		s_env = NULL;
		s_env = env_build(env, s_env);
		if ((search = search_env(s_env, "SHLVL")))
		{
			tmp = search->value;
			search->value = ft_itoa(ft_atoi(tmp) + 1);
			ft_strdel(&tmp);
		}
		show_prompt(s_env, data);
	}
	else
		ft_putstr_fd("minishell: can't open input file\n", 2);
	destroy_env(&s_env);
	return (data->rvalue);
}
