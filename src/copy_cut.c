/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cut.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/25 17:16:39 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 17:51:50 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	standout_move(t_data *data, char *stin, char *buff)
{
	if (stin && data->cursor >= 0 && data->cursor < (int)ft_strlen(stin))
	{
		act_pos(data);
		ft_putchar(stin[data->cursor]);
		if (data->col != data->scr_col)
			tputs(data->a->le_s, 1, print);
	}
	if (buff[2] == 'C' && data->col == data->scr_col &&
			data->cursor < (int)ft_strlen(stin))
	{
		tputs(data->a->do_s, 1, print);
		data->cursor++;
	}
	else if (buff[2])
		move_cursor(data, stin, buff);
}

static void	copy(t_data *data, char *stin, int ce)
{
	if (stin)
	{
		ft_strdel(&data->clipboard);
		if (ce >= data->cursor)
			data->clipboard = ft_strsub(stin, data->cursor,
					ce - data->cursor + 1);
		else if (ce < data->cursor)
			data->clipboard = ft_strsub(stin, ce, data->cursor - ce + 1);
	}
}

static void	cut(t_data *data, char **stin, int ce)
{
	char *tmp;

	tmp = *stin;
	if (*stin)
	{
		ft_strdel(&data->clipboard);
		if (ce >= data->cursor)
		{
			data->clipboard = ft_strsub(*stin, data->cursor,
					ce - data->cursor + 1);
			*stin = join(ft_strsub(tmp, 0, data->cursor), &tmp[ce + 1], "");
		}
		else if (ce < data->cursor)
		{
			data->clipboard = ft_strsub(*stin, ce, data->cursor - ce + 1);
			*stin = join(ft_strsub(tmp, 0, ce), &tmp[data->cursor], "");
		}
		ft_strdel(&tmp);
	}
}

static int	copy_cut_key(char *buff, t_data *data, char **stin, int ce)
{
		if (buff[0] == 27 && buff[1] == 91)//arrow
		{
			standout_move(data, *stin, buff);
			return (0);
		}
		else if (buff[0] == 21 && buff[1] == 0)// copy  |   CTRL + U
		{
			copy(data, *stin, ce);
			reset_line(data, *stin);
		}
		else if (buff[0] == 11 && buff[1] == 0)// cut       |      CTRL + K
		{
			cut(data, stin, ce);
			reset_line(data, *stin);
		}
		else if (buff[0] == 27 && buff[1] == 0)//echap
			reset_line(data, *stin);
		return (1);
}

void		copy_cut(t_data *data, char **stin, char *buff)
{
	int		ce;

	tputs(data->a->sc_s, 1, print);
	tputs(data->a->so_s, 1, print);
	ce = data->cursor;
	while (42)
	{
		act_pos(data);
		ft_bzero(buff, 6);
		read(0, buff, 5);
		if (copy_cut_key(buff, data, stin, ce))
			break ;
	}
	int t;
	if (ce > data->cursor)
	{
		t = ce - data->cursor;
		data->cursor = ce;
		while (t > 0)
		{
			move_left(data);
			t--;
		}
	}
	else
		data->cursor = ce;
	tputs(data->a->se_s, 1, print);
}
