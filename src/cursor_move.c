/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 19:43:43 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/21 16:48:18 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	go_home(t_data *data)
{
	while (data->cursor != 0)
	{
		act_pos(data);
		if (data->col == 1)
			tputs(tgoto(data->a->cm_s,
						data->scr_col - 1, data->row - 2), 1, print);
		else
			tputs(data->a->le_s, 1, print);
		data->cursor--;
	}
}

void		go_end(t_data *data, char *stin)
{
	while (data->cursor < (int)ft_strlen(stin))
	{
		act_pos(data);
		if (data->col == data->scr_col)
			tputs(data->a->do_s, 1, print);
		else
			tputs(data->a->nd_s, 1, print);
		data->cursor++;
	}
}

void	move_left(t_data *data)
{
	act_pos(data);
	if (data->col == 1)
		tputs(tgoto(data->a->cm_s,
					data->scr_col - 1, data->row - 2), 1, print);
	else
		tputs(data->a->le_s, 1, print);
	data->cursor--;
}

void	move_right(t_data *data)
{
	act_pos(data);
	if (data->col == data->scr_col)
		tputs(data->a->do_s, 1, print);
	else
		tputs(data->a->nd_s, 1, print);
	data->cursor++;
}
