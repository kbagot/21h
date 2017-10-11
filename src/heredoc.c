/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 17:34:09 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/10 21:35:26 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	heredoc(t_data *d, char **s, int j, int i)
{
	char	*line;
	char	*tmp;
	int		fd[2];

	pipe(fd);
	ft_putstr("> ");
	while (42)
	{
		line = line_edit(d);
		if (line && (!ft_strcmp(line, s[i + 1]) || !ft_strcmp(line, "exit")))
		{
			ft_strdel(&line);
			break ;
		}
		ft_putstr_fd(line, fd[1]);
		ft_putchar_fd('\n', fd[1]);
		ft_strdel(&line);
		ft_putstr("> ");
	}
	close(fd[1]);
	tmp = s[i];
	s[i] = join(ft_strsub(s[i], 0, j), "&", "");
	ft_strdel(&tmp);
	ft_strdel(&s[i + 1]);
	s[i + 1] = ft_itoa(fd[0]);
}

void		creat_heredoc(t_data *d, char **s)
{
	int i;
	int j;

	d->in = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i][0] == '\"' || s[i][0] == '\'')
			i++;
		else
		{
			j = 0;
			while (s[i][j])
			{
				if (!ft_strncmp(&s[i][j], "<<", 2))
					heredoc(d, s, j + 1, i);
				j++;
			}
			i++;
		}
	}
}
