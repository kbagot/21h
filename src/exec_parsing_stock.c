/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parsing_stock.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:29:58 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/11 20:40:36 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	stock_redir_fd(t_line *line, char **cstin, int *k, char *c)
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

static void	stock_redir(char **cstin, t_line *line, int *j, int *k)
{
	char	*c;
	char	*nbr;

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
		if (!ft_isnbr((nbr = ft_strsub(*cstin, 0, c - *cstin))))
		{
			line->proc[*j] = ft_strsub(*cstin, 0, c - *cstin);
			line->redirect[*k] = join(ft_strdup(&cstin[0][c - *cstin]), " ",
					cstin[1]);
			*j += 1;
			*k += 1;
		}
		else
			stock_redir_fd(line, cstin, k, c);
		ft_strdel(&nbr);
	}
}

static void	sep_line_stck(t_line *line, int *j, char **cstin, int *i)
{
	if (ft_strchr("\'\"", cstin[*i][0]))
		line->proc[*j] = ft_strdup(&cstin[*i][1]);
	else
		line->proc[*j] = ft_strdup(cstin[*i]);
	*j += 1;
}

void		separate_line(char **cstin, int *i, t_line *line)
{
	int		j;
	int		k;
	char	*c;

	k = 0;
	j = 0;
	while (cstin[*i] && (ft_strcmp(cstin[*i], "|") != 0))
	{
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
