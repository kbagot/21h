/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/03 16:56:01 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/03 17:08:20 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	destroy_env(t_env **env)
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
