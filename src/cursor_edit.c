/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/21 18:27:12 by kbagot            #+#    #+#             */
/*   Updated: 2017/08/26 18:46:32 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	print_hist(char **stin, t_data *data)
{
	ft_printf("%s", *stin);
	tputs(tgetstr("sc", NULL), 1, print);
	cleaner(data);
	tputs(tgetstr("rc", NULL), 1, print);
	data->cursor = ft_strlen(*stin);
}

static int	history_browsing(t_data *data, char **stin, char *buff)
{
	if (buff[2] == tgetstr("ku", NULL)[2] && data->hist)
	{
		go_home(data);
		ft_strdel(stin);
		*stin = ft_strdup(data->hist->elem);
		print_hist(stin, data);
		if (data->hist->before)
			data->hist = data->hist->before;
		return (0);
	}
	else if (buff[2] == tgetstr("kd", NULL)[2] && data->hist)
	{
		if (data->hist->next)
		{
			go_home(data);
			ft_strdel(stin);
			*stin = ft_strdup(data->hist->next->elem);
			print_hist(stin, data);
			data->hist = data->hist->next;
		}
		return (0);
	}
	return (1);
}

static int	move_cursor(t_data *data, char *stin, char *buff)
{
	if (buff[2] == tgetstr("kr", NULL)[2] && stin &&
			data->cursor < (int)ft_strlen(stin))
	{
		move_right(data);
		return (0);
	}
	else if (buff[2] == tgetstr("kl", NULL)[2] && data->cursor > 0)
	{
		move_left(data);
		return (0);
	}
	return (1);
}

static void	home_end_key(t_data *data, char *stin, char *buff)
{
	if (buff[2] == 72 && data->cursor != 0)
		go_home(data);
	else if (buff[2] == 70 && stin)
		go_end(data, stin);
}

void		arrow_key(t_data *data, char **stin, char *buff)
{
	if (buff[2])
	{
		if (history_browsing(data, stin, buff))
			if (move_cursor(data, *stin, buff))
				home_end_key(data, *stin, buff);
	}
}
