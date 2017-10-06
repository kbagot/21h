/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_by_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 17:39:59 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/06 16:14:17 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	word_left(t_data *data, char *stin)
{
	if ((data->cursor > 0 && ft_isspace(stin[data->cursor - 1]) &&
				stin[data->cursor] > 32) || !stin[data->cursor])
		move_left(data);
	while (stin[data->cursor] && data->cursor > 0)
	{
		if (ft_isspace(stin[data->cursor - 1]) && stin[data->cursor] > 32)
			break ;
		move_left(data);
	}
}

static void	word_right(t_data *data, char *stin)
{
	if ((data->cursor == 0 || ft_isspace(stin[data->cursor - 1])) &&
			stin[data->cursor] > 32)
		move_right(data);
	while (stin[data->cursor] && data->cursor < (int)ft_strlen(stin))
	{
		if (ft_isspace(stin[data->cursor - 1]) && stin[data->cursor] > 32)
			break ;
		move_right(data);
	}
}

static void	line_up(t_data *data)
{
	int line;

	line = data->scr_col;
	while (data->cursor > 0 && line > 0)
	{
		move_left(data);
		line--;
	}
}

static void	line_down(t_data *data, char *stin)
{
	int line;

	line = data->scr_col;
	while (stin[data->cursor] && line > 0)
	{
		move_right(data);
		line--;
	}
}

void		move_by_word_line(t_data *data, char *stin, char *buff)
{
	if (buff[0] == 6 && stin && data->cursor > 0)
		word_left(data, stin);
	else if (buff[0] == 2 && stin)
		word_right(data, stin);
	else if (buff[0] == 19 && stin)
		line_up(data);
	else if (buff[0] == 24 && stin)
		line_down(data, stin);
}
