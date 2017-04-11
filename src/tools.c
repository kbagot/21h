/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/30 15:38:20 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/06 15:24:35 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	swap_dat(char **dat, int i)
{
	int j;

	j = 0;
	while (j < i)
	{
		ft_strdel(&dat[j]);
		j++;
	}
	j = 0;
	while (dat[i])
	{
		dat[j] = dat[i];
		i++;
		j++;
	}
	dat[j] = NULL;
}

char	**list_to_tab(t_env *lenv)
{
	char	*env;
	char	*tmp;
	char	**dest;

	tmp = NULL;
	dest = NULL;
	env = ft_strnew(1);
	while (lenv)
	{
		tmp = ft_strjoin(env, lenv->name);
		ft_strdel(&env);
		env = ft_strjoin(tmp, "=");
		ft_strdel(&tmp);
		tmp = ft_strjoin(env, lenv->value);
		ft_strdel(&env);
		env = ft_strjoin(tmp, " ");
		ft_strdel(&tmp);
		lenv = lenv->next;
	}
	dest = ft_strsplit(env, ' ');
	ft_strdel(&env);
	return (dest);
}

void	delete_env(t_env **env_p, char *name)
{
	t_env *bef;
	t_env *env;

	bef = NULL;
	env = *env_p;
	while (env)
	{
		if ((ft_strcmp(env->name, name)) == 0)
		{
			if (bef == NULL)
				*env_p = env->next;
			else if (bef)
				bef->next = env->next;
			ft_strdel(&env->name);
			ft_strdel(&env->value);
			free(env);
			env = NULL;
			return ;
		}
		bef = env;
		env = env->next;
	}
}

void	add_env(t_env *env, char **cstin)
{
	t_env	*search;

	if (cstin[0] && (search = search_env(env, cstin[0])))
		if (search && cstin[0])
		{
			ft_strdel(&search->value);
			if (cstin[1])
				search->value = ft_strdup(cstin[1]);
			else
				search->value = ft_strnew(1);
			return ;
		}
	while (env->next)
		env = env->next;
	if (cstin[0])
	{
		env->next = ft_memalloc(sizeof(t_env));
		env->next->name = ft_strdup(cstin[0]);
		if (cstin[1])
			env->next->value = ft_strdup(cstin[1]);
		else
			env->next->value = ft_strnew(1);
	}
	else
		ft_putstr_fd("usage: setenv [name=value ...]\n", 2);
}

t_env	*search_env(t_env *env, char *name)
{
	while (env)
	{
		if ((ft_strcmp(env->name, name)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
