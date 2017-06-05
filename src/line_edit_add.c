/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 15:41:47 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/05 19:40:44 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	paste(t_data *data, char **stin)
{

	if (data->clipboard)
		writer(data, stin, data->clipboard);
}

void	writer(t_data *data, char **stin, char *buff)
{
	char *tmp;

	act_pos(data);
	tmp = *stin;
	if (*stin == NULL)
		*stin = ft_strdup(buff);
	else if (data->cursor == (int)ft_strlen(*stin)) // maybe -1
		*stin = ft_strjoin(tmp, buff);
	else
		*stin = join(ft_strsub(tmp, 0, data->cursor), buff, &tmp[data->cursor]);
	if (buff[0] == 9)
		buff[0] = 32;
	ft_strdel(&tmp);
	tmp = *stin;
	int lol;
	int i;
	int len;
	len = ft_strlen(buff);
	i = data->cursor;
	tputs(tgetstr("sc", NULL), 1, print);
	lol = data->row + (((data->col + (((int)ft_strlen(&stin[0][data->cursor]) - 2))) / data->scr_col));		
	ft_putstr(&stin[0][data->cursor]);
	act_pos(data);
	if (lol >= data->scr_row + 1) /*&& ((int)ft_strlen(*stin) + data->start_col - 2) % data->scr_col == 0)*/
	{
		//ft_printf("%d %d %d", lol, data->row, data->scr_row);
		tputs(tgetstr("rc", NULL), 1, print);
		act_pos(data);
		tputs(tgetstr("up", NULL), 1, print);
	}
	else
		tputs(tgetstr("rc", NULL), 1, print);
	data->cursor += len;
	while (len > 0)
	{
		act_pos(data);
		if (data->col == data->scr_col)
			tputs(tgetstr("do", NULL), 1, print);
		else
			tputs(tgetstr("nd", NULL), 1, print);
		len--;
	}
}
