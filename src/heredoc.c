/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 17:34:09 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 20:57:36 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	heredoc(t_data *d, char **s, int j, int i)
{
	char *line;
	//	int up;
	int fd[2];

	pipe(fd); // save fd ,  he use reserved fd
	//	if (j != 0)
	//		up = ft_atoi(*s);
	//	else
	//		up = 0;
	ft_putstr("> ");
	//	printf("%s\n", cmd[i]);
	while (42)
	{
		line = line_edit(d);
		if (line && (!ft_strcmp(line, s[i + 1]) || !ft_strcmp(line, "exit")))
			break ;
		ft_putstr_fd(line, fd[1]);
		ft_putchar_fd('\n', fd[1]);
		ft_strdel(&line);
		ft_putstr("> ");
	}
	close(fd[1]);
	s[i] = ft_strjoin(ft_strsub(s[i], 0, j), "&");
	s[i + 1] = ft_itoa(fd[0]);
	//	printf("%s\n", s[i]);
	//dup2(fd[0], up);
	//close(fd[0]);
}

void	creat_heredoc(t_data *d, char **s)
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
				//	printf("%s\n", s[i]);}
				j++;
			}
		i++;
		}
	}
}
