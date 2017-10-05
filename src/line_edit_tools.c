/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/05 16:47:45 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 20:40:09 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

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
		tputs(data->a->do_s, 1, print);
		while (data->row != data->scr_row)
		{
			tputs(data->a->dl_s, 1, print);
			data->row++;
		}
	}
}

void	reset_line(t_data *data, char *stin)
{
	int cursor;

	cursor = data->cursor;
	tputs(data->a->se_s, 1, print);
	if (stin)
	{
		go_home(data);
		ft_printf("%s", stin);
		cleaner(data);
	}
	data->cursor = cursor;
	tputs(data->a->rc_s, 1, print);
}
