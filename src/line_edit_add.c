/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 15:41:47 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/21 16:45:06 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	mod_stock(char **stin, t_data *data, char *buff)
{
	char *tmp;

	act_pos(data);
	tmp = *stin;
	if (*stin == NULL)
		*stin = ft_strdup(buff);
	else if (data->cursor == (int)ft_strlen(*stin))
		*stin = ft_strjoin(tmp, buff);
	else
		*stin = join(ft_strsub(tmp, 0, data->cursor), buff, &tmp[data->cursor]);
	if (buff[0] == 9) // not the good way
		buff[0] = 32;
	ft_strdel(&tmp);
	tmp = *stin;
}

static void	multi_line(char **stin, t_data *data, int len)
{
	int end;

	tputs(data->a->sc_s, 1, print);
	end = data->row + (((data->col +
(((int)ft_strlen(&stin[0][data->cursor]) - 2))) / data->scr_col));
	ft_putstr(&stin[0][data->cursor]);
	act_pos(data);
	if (end >= data->scr_row + 1)
	{
		tputs(data->a->rc_s, 1, print);
		act_pos(data);
		tputs(data->a->up_s, 1, print);
	}
	else
		tputs(data->a->rc_s, 1, print);
	data->cursor += len;
	while (len > 0)
	{
		move_right(data);
		data->cursor--;
		len--;
	}
}

void		writer(t_data *data, char **stin, char *buff)
{
	int len;

	len = ft_strlen(buff);
	mod_stock(stin, data, buff);
	if (data->scr_col == 0)
		data->scr_col = 1;
	multi_line(stin, data, len);
}
