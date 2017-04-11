/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/07 14:37:51 by kbagot            #+#    #+#             */
/*   Updated: 2016/12/21 18:26:47 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_makeline(char *lst, char **line)
{
	int		i;
	int		fin;

	fin = 0;
	i = 0;
	if (lst[i] != '\0')
	{
		while (lst[i] != '\n' && lst[i])
			i++;
		if (lst[i] == '\0')
			fin = 1;
		lst[i] = '\0';
		*line = ft_strnew(i);
		*line = ft_strcpy(*line, lst);
		lst = ft_strcpy(lst, &lst[i + 1]);
		if (fin == 1)
			lst[0] = '\0';
		return (1);
	}
	return (0);
}

static t_list	*ft_find_fd(t_list *rstr, int fd)
{
	t_list	*new;
	t_list	*tmp;

	tmp = NULL;
	if (rstr == NULL)
	{
		rstr = ft_lstnew(NULL, fd);
		rstr->content_size = fd;
		rstr->content = ft_strnew(BUFF_SIZE);
		return (rstr);
	}
	while (rstr)
	{
		tmp = rstr;
		if (rstr->content_size == (unsigned int)fd)
			return (rstr);
		rstr = rstr->next;
	}
	rstr = tmp;
	new = ft_lstnew(NULL, fd);
	new->content_size = fd;
	new->content = ft_strnew(BUFF_SIZE);
	new->next = NULL;
	rstr->next = new;
	return (new);
}

static int		ft_stock(t_list *rstr, int fd)
{
	char	*buff;
	char	*stop;
	int		ret;
	char	*test;

	stop = NULL;
	buff = ft_strnew(BUFF_SIZE);
	test = rstr->content;
	while (stop == NULL && (ret = read(fd, buff, BUFF_SIZE)))
	{
		if (ret == -1)
			return (-1);
		ft_bzero(&buff[ret], BUFF_SIZE - ret);
		stop = ft_strchr(buff, '\n');
		rstr->content = ft_strjoin(test, buff);
		free(test);
		test = rstr->content;
	}
	free(buff);
	buff = NULL;
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*rstr = NULL;
	t_list			*saverstr;

	saverstr = NULL;
	if (fd < 0 || !line || BUFF_SIZE <= 0)
		return (-1);
	saverstr = rstr;
	rstr = ft_find_fd(rstr, fd);
	if (ft_stock(rstr, fd) == -1)
		return (-1);
	if (ft_makeline(rstr->content, line))
	{
		if (saverstr)
			rstr = saverstr;
		return (1);
	}
	if (saverstr)
		rstr = saverstr;
	return (0);
}
