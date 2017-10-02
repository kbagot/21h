/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 21:08:28 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 21:11:53 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		stock_redir_fd(t_line *line, char **cstin, int *k, char *c)
{
	char *tmp;
	char *tmp2;

	tmp = ft_strsub(cstin[0], 0, c - cstin[0]);
	tmp2 = ft_strjoin(c, " ");
	line->redirect[*k] = join(ft_strjoin(tmp, " "), tmp2, cstin[1]);
	ft_strdel(&tmp);
	ft_strdel(&tmp2);
	*k += 1;
}

static void		stock_redir(char **cstin, t_line *line, int *j, int *k)
{
	char *c;

	c = NULL;
	if (!(c = ft_strchr(*cstin, '>')))
		c = ft_strchr(*cstin, '<');
	if (c == *cstin)
	{
		line->redirect[*k] = join(ft_strdup(*cstin), " ", cstin[1]);
		*k += 1;
	}
	else
	{
		if (!ft_isnbr(ft_strsub(*cstin, 0, c - *cstin)))
		{
			line->proc[*j] = ft_strsub(*cstin, 0, c - *cstin);
			line->redirect[*k] = join(ft_strdup(&cstin[0][c - *cstin]), " ",
					cstin[1]);
			*j += 1;
			*k += 1;
		}
		else
			stock_redir_fd(line, cstin, k, c);
	}
}

static void		sep_line_stck(t_line *line, int *j, char **cstin, int *i)
{
	if (ft_strchr("\'\"", cstin[*i][0]))
		line->proc[*j] = ft_strdup(&cstin[*i][1]);
	else
		line->proc[*j] = ft_strdup(cstin[*i]);
	*j += 1;
}

static void		separate_line(char **cstin, int *i, t_line *line)
{
	int		j;
	int		k;
	char	*c;

	k = 0;
	j = 0;
	while (cstin[*i] && (ft_strcmp(cstin[*i], "|") != 0))
	{//add redirect cmd
		if (!ft_strchr("\'\"", cstin[*i][0]) && ((c = ft_strchr(cstin[*i], '>'))
					|| (c = ft_strchr(cstin[*i], '<'))))
		{
			stock_redir(&cstin[*i], line, &j, &k);
			*i += 1;
		}
		else
			sep_line_stck(line, &j, cstin, i);
		if (!cstin[*i])
			break ;
		*i += 1;
	}
	line->redirect[k] = NULL;
	line->proc[j] = NULL;
}

static void		find_pipe(char **cstin, int *j, int *k)
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

t_line	*split_pipe(char **cstin)
{
	t_line	*line;
	t_line	*save;
	int		i;
	int		j;
	int		k;

	save = NULL;
	i = 0;
	j = 0;
	k = 0;
	while (cstin[i])
	{
		if (save == NULL)
		{
			line = ft_memalloc(sizeof(t_line));
			save = line;
		}
		find_pipe(&cstin[i], &j, &k);
		line->proc = (char**)malloc(sizeof(char*) * (j + 1));
		line->redirect = (char**)malloc(sizeof(char*) * (k + 1));
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
