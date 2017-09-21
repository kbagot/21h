/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cut.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/25 17:16:39 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/21 17:53:02 by kbagot           ###   ########.fr       */
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
	if (buff[2] == 'C' && data->col == data->scr_col && data->cursor < (int)ft_strlen(stin))
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
		ft_strdel(&data->clipboard);//check if good free
		if (ce >= data->cursor)
		{
			data->clipboard = ft_strsub(*stin, data->cursor,
					ce - data->cursor + 1);
			*stin = ft_strjoin(ft_strsub(tmp, 0, data->cursor), &tmp[ce + 1]); 
			//stin leaks
		}
		else if (ce < data->cursor)
		{
			data->clipboard = ft_strsub(*stin, ce, data->cursor - ce + 1);
			*stin = ft_strjoin(ft_strsub(tmp, 0, ce), &tmp[data->cursor]); //stin leaks
		}
		ft_strdel(&tmp);
	}
}

void	cleaner(t_data *data)
{
	int ll;
	ll = 0;

	act_pos(data);
	ll = data->scr_col - data->col + 1;
	while (ll != 0)
	{
		tputs(data->a->dc_s, 1, print);
		ll--;
	}
	if (data->row != data->scr_row)
	{
		while (data->row != data->scr_row)
		{
			act_pos(data);
			if (data->row != data->scr_row)
				tputs(data->a->do_s, 1, print);
			tputs(data->a->dl_s, 1, print);
		}
	}
}

void	reset_line(t_data *data, char *stin)
{
	int cursor;
	tputs(data->a->se_s, 1, print);
	cursor = data->cursor;
	if (stin)
	{
		go_home(data);
		ft_printf("%s ", stin);
		cleaner(data);
	}
	data->cursor = cursor;
	tputs(data->a->rc_s, 1, print);
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
		if (buff[0] == 27 && buff[1] == 91)//arrow
			standout_move(data, *stin, buff);
		else if (buff[0] == 21 && buff[1] == 0)// copy  |   CTRL + U
		{
			copy(data, *stin, ce);
			reset_line(data, *stin);
			break ;
		}
		else if (buff[0] == 11 && buff[1] == 0)// cut       |      CTRL + K
		{
			cut(data, stin, ce);
			reset_line(data, *stin);
			break ;
		}
		else if (buff[0] == 27 && buff[1] == 0)//echap
		{
			reset_line(data, *stin);
			break ;
		}
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
