/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/06 18:20:37 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/07 12:22:29 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	up_pwd(t_env *pwd, t_env *oldpwd, char **cstin)
{
	struct stat	buf;
	char		*tmp;

	tmp = NULL;
	ft_strdel(&pwd->value);
	if (oldpwd)
		tmp = join(ft_strdup(oldpwd->value), "/", cstin[1]);
	if (oldpwd && (lstat(tmp, &buf) == 0) && S_ISLNK(buf.st_mode) == 1 &&
			ft_strcmp(cstin[1], "-P") != 0)
		pwd->value = tmp;
	else
	{
		pwd->value = ft_strnew(PATH_MAX);
		getcwd(pwd->value, PATH_MAX);
		ft_strdel(&tmp);
	}
}

static void	update_env(t_env *env, char **cstin)
{
	t_env		*pwd;
	t_env		*oldpwd;

	oldpwd = NULL;
	pwd = search_env(env, "PWD");
	if (pwd && (oldpwd = search_env(env, "OLDPWD")))
	{
		ft_strdel(&oldpwd->value);
		oldpwd->value = ft_strdup(pwd->value);
	}
	if (pwd)
		up_pwd(pwd, oldpwd, cstin);
}

int			exec_cd(char **cstin, t_env *env)
{
	t_env	*search;
	int		i;

	i = 0;
	while (cstin[i])
		i++;
	if (i >= 4)
	{
		ft_putstr_fd("cd: Too many arguments\n", 2);
		return (1);
	}
	else if (i == 2 && ft_strcmp(cstin[1], "-") == 0 &&
			(search = search_env(env, "OLDPWD")))
		chdir(search->value);
	else if ((cstin[1] && ft_strcmp(cstin[1], "-") == 0) ||
			((i == 2 || i == 3) && (chdir(cstin[i - 1])) == -1))
	{
		ft_putstr_fd("cd: No such file or directory\n", 2);
		return (1);
	}
	else if (i == 1 && (search = search_env(env, "HOME")))
		chdir(search->value);
	update_env(env, cstin);
	return (1);
}
