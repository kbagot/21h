/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 15:14:05 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/06 19:39:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char		*join(char *s1, char *s2, char *s3)
{
	char *tmp;
	char *new;

	tmp = ft_strjoin(s1, s2);
	ft_strdel(&s1);
	new = ft_strjoin(tmp, s3);
	ft_strdel(&tmp);
	return (new);
}

static char	**path_init(t_env *s_env)
{
	char	**path;
	t_env	*search;
	char	*tmp;

	path = NULL;
	if ((search = search_env(s_env, "PATH")))
	{
		tmp = ft_strjoin(search->value, ":");
		path = ft_strsplit(tmp, ':');
		ft_strdel(&tmp);
	}
	else
		path = ft_strsplit(_PATH_DEFPATH, ':');
	return (path);
}

char		**utility(char **st, t_env *s_env)
{
	int		i;
	char	**path;

	i = 0;
	path = path_init(s_env);
	while (path && path[i])
	{
		path[i] = join(path[i], "/", st[0]);
		if ((access(path[i], X_OK)) == 0)
		{
			ft_strdel(&st[0]);
			st[0] = ft_strdup(path[i]);
			ft_tabdel(path);
			return (st);
		}
		i++;
	}
	ft_tabdel(path);
	return (st);
}
