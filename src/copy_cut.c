/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cut.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/25 17:16:39 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/05 19:40:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	standout_move(t_data *data, char *stin, char *buff)
{
	if (stin && data->cursor >= 0 && data->cursor < (int)ft_strlen(stin))
	{
		ft_putchar(stin[data->cursor]);
		tputs(tgetstr("le", NULL), 1, print);
	}
	arrow_key(data, &stin, buff);
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
			*stin = ft_strjoin(ft_strsub(tmp, 0, data->cursor), &tmp[ce + 1]); //stin leaks
		}
		else if (ce < data->cursor)
		{
			data->clipboard = ft_strsub(*stin, ce, data->cursor - ce + 1);
			*stin = ft_strjoin(ft_strsub(tmp, 0, ce), &tmp[data->cursor]); //stin leaks
		}
		ft_strdel(&tmp);
	}
}

static void	reset_line(t_data *data, char *stin)
{
	int ll;

	ll = 0;
	tputs(tgetstr("se", NULL), 1, print);
	if (stin)
	{
		go_home(data);
		ft_printf("%s", stin);
		act_pos(data);
		ll = data->scr_col - data->col + 1;
		while (ll != 0)
		{
			tputs(tgetstr("dc", NULL), 1, print);
			ll--;
		}
		if (data->row != data->scr_row)
		{
			while (data->row != data->scr_row)
			{
				act_pos(data);
				if (data->row != data->scr_row)
				tputs(tgetstr("do", NULL), 1, print);
				tputs(tgetstr("dl", NULL), 1, print);
			}
		}
	}
	tputs(tgetstr("rc", NULL), 1, print);
}

void		copy_cut(t_data *data, char **stin, char *buff)
{
	int		ce;

	tputs(tgetstr("sc", NULL), 1, print);
	tputs(tgetstr("so", NULL), 1, print);
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
			break ;
	}
	data->cursor = ce;
	tputs(tgetstr("se", NULL), 1, print);
}
