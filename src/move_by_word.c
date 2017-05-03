/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_by_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 17:39:59 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/27 19:45:22 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	word_left(t_data *data, char *stin)
{
	if ((data->cursor > 0 && ft_isspace(stin[data->cursor - 1]) &&
				stin[data->cursor] > 32) || !stin[data->cursor])
	{
		tputs(tgetstr("le", NULL), 1, print);
		data->cursor--;
	}
	while (stin[data->cursor] && data->cursor > 0)
	{
		if (ft_isspace(stin[data->cursor - 1]) && stin[data->cursor] > 32)
			break ;
		tputs(tgetstr("le", NULL), 1, print);
		data->cursor--;
	}
}

static void	word_right(t_data *data, char *stin)
{
	if ((data->cursor == 0 || ft_isspace(stin[data->cursor - 1])) &&
			stin[data->cursor] > 32)
	{
		tputs(tgetstr("nd", NULL), 1, print);
		data->cursor++;
	}
	while (stin[data->cursor] && data->cursor > 0)
	{
		if (ft_isspace(stin[data->cursor - 1]) && stin[data->cursor] > 32)
			break ;
		tputs(tgetstr("nd", NULL), 1, print);
		data->cursor++;
	}
}

void		move_by_word(t_data *data, char *stin, char *buff)
{
	if (buff[0] == 6 && stin && data->cursor > 0)
		word_left(data, stin);
	else if (buff[0] == 2 && stin)
		word_right(data, stin);
}
