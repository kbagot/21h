/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 14:36:14 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/06 20:19:34 by kbagot           ###   ########.fr       */
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

static int	parse_token(char **stin)
{
	int i;
	char *c;

	i = 0;
	while (stin[i])
	{
if ((c = ft_strchr(stin[i], '&')) && (c[1] != '<' && c[1] != '>')&& (c - stin[i] > 0 && c[-1] != '<' && c[-1] != '>'))
		{
			if (!stin[i + 1])
				ft_putstr_fd("21sh: syntax error near unexpected token `newline'\n", 2);
			else
				ft_putstr_fd("21sh: syntax error near unexpected token `&'\n", 2);
				return (1);
		}
		else if (!ft_strcmp(stin[i], "&"))
		{
			ft_putstr_fd("321sh: syntax error near unexpected token `&'\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	parse_error(char **stin)
{//TOKEN
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (stin && stin[i])
	{
		tmp = strmsplit(stin[i], " \t\n");
		if (!tmp[0])
		{
			ft_tabdel(&tmp);
			return (0);
		}
		j = 0;
		if (parse_token(tmp))
			{ft_tabdel(&tmp);
			return (0);}
		while (tmp[j])
			j++;
		if (strcmp(tmp[0], "|") == 0 || (j > 0 && strcmp(tmp[j - 1], "|") == 0))
		{
			ft_tabdel(&tmp);
			ft_putstr_fd("21sh: error near unexpected token `|'\n", 2);
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
		septin = strquotesplit(stin, ";");//use stin fot history
		if (parse_error(septin)) // token
		{
			while (stin && septin[i])
			{
				cstin = splitforquote(septin[i], " \t\n");
				//cstin = strquotesplit(septin[i], " \t\n");
				//int lol;
				//lol=0;
				//while (cstin[lol])
				//{ft_printf("[%s]\n", cstin[lol]);
				//lol++;}
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
