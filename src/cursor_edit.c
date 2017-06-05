/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/21 18:27:12 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/05 14:03:33 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	history_browsing(t_data *data, char **stin, char *buff)
{
	if (buff[2] == tgetstr("ku", NULL)[2] && data->hist)
	{
		ft_strdel(stin);
		*stin = ft_strdup(data->hist->elem);
		tputs(tgetstr("rc", NULL), 1, print);
		tputs(tgetstr("ce", NULL), 1, print);
		ft_printf("%s", *stin);
		data->cursor = ft_strlen(*stin);
		if (data->hist->before)
			data->hist = data->hist->before;
		return (0);
	}
	else if (buff[2] == tgetstr("kd", NULL)[2] && data->hist)
	{
		if (data->hist->next)
		{
			ft_strdel(stin);
			*stin = ft_strdup(data->hist->next->elem);
			tputs(tgetstr("rc", NULL), 1, print);
			tputs(tgetstr("ce", NULL), 1, print);
			ft_printf("%s", *stin);
			data->cursor = ft_strlen(*stin);
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
		if (data->col == data->scr_col)
			tputs(tgetstr("do", NULL), 1, print);
		else
			tputs(tgetstr("nd", NULL), 1, print);
		data->cursor++;
		return (0);
	}
	else if (buff[2] == tgetstr("kl", NULL)[2] && data->cursor > 0)
	{
		if (data->col == 1)
			tputs(tgoto(tgetstr("cm", NULL),
						data->scr_col - 1, data->row - 2), 1, print);
		else
			tputs(tgetstr("le", NULL), 1, print);
		data->cursor--;
		return (0);
	}
	return (1);
}


void	go_home(t_data *data)
{
		while (data->cursor != 0)
		{
			act_pos(data);
			if (data->col == 1)
				tputs(tgoto(tgetstr("cm", NULL),
							data->scr_col - 1, data->row - 2), 1, print);
			else
				tputs(tgetstr("le", NULL), 1, print);
			data->cursor--;
		}
}

void		go_end(t_data *data, char *stin)
{
		while (data->cursor < (int)ft_strlen(stin))
		{
			act_pos(data);
			if (data->col == data->scr_col)
				tputs(tgetstr("do", NULL), 1, print);
			else
				tputs(tgetstr("nd", NULL), 1, print);
			data->cursor++;
		}
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
