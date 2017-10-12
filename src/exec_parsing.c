/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 21:08:28 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/12 14:59:01 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	find_pipe(char **cstin, int *j, int *k)
{
	int i;

	i = 0;
	while (cstin[i] && ft_strcmp(cstin[i], "|") != 0)
	{
		if (!ft_strchr("\"\'", cstin[i][0] && (ft_strchr(cstin[i], '>') ||
						ft_strchr(cstin[i], '<'))))
			*k += 1;
		else
			*j += 1;
		i++;
	}
}

static void	init_pipe(int i, char **cstin, t_line *line)
{
	int j;
	int k;

	j = 0;
	k = 0;
	find_pipe(&cstin[i], &j, &k);
	line->proc = (char**)malloc(sizeof(char*) * (j + 1));
	line->redirect = (char**)malloc(sizeof(char*) * (k + 1));
}

t_line		*split_pipe(char **cstin)
{
	t_line	*line;
	t_line	*save;
	int		i;

	i = 0;
	save = NULL;
	while (cstin[i])
	{
		if (save == NULL)
		{
			line = ft_memalloc(sizeof(t_line));
			save = line;
		}
		init_pipe(i, cstin, line);
		separate_line(cstin, &i, line);
		line->next = NULL;
		if (!cstin[i])
			break ;
		line->next = ft_memalloc(sizeof(t_line));
		line = line->next;
		i++;
	}
	return (save);
}
