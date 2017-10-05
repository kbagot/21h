/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 14:37:42 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 22:15:01 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	l_edit_1(char *buff, char **stin, t_data *data)
{
	if (buff[0] == 27 && buff[1] == 91)//arrow
		arrow_key(data, stin, buff);
	else if (buff[0] == 6 || buff[0] == 2)// ctrl + F  ctr+B
		move_by_word(data, *stin, buff);
	else if ((buff[0] == 25 || buff[0] == 11) && buff[1] == 0)//c mode ctrl+u k
		copy_cut(data, stin, buff);
	else if (buff[0] == 16 && buff[1] == 0)// PASTE   CTRL+P
			paste(data, stin);
	else
		return (1);
	return (0);
}

static int	l_edit_2_enterkey(char **stin, char *buff, t_edit *e, t_data *data)
{
	if (get_proc(0) == 1)
	{
		ft_strdel(stin);
		get_proc(2);
	}
	if (buff[0] == 12)
		tputs(data->a->cl_s, 1, print);
	if (buff[0] == 10 && conform_quote(*stin) != 0) //mutli line issue
		ft_putstr("\n> ");
	else
	{
		if (buff[0] != 12)
			ft_putchar('\n');
		add_history(e->stin, data);
		//end_line(data, *stin, buff);
		*stin = conform(*stin);
		return (1);
	}
	return (0);
}

static int	l_edit_2(char *buff, char **stin, t_edit *e, t_data *data)
{
	if ((!stin || (stin && stin[0] == '\0')) && buff[0] == 4)//ctrl + d [make exit]
	{
		ft_strdel(stin);
		*stin = ft_strdup("exit");
		//end_line(data, *stin, buff);
		return (2);
	}
	else if (buff[0] == 10 || buff[0] == 12) // send enter / ctrl+L clear
	{
		if (l_edit_2_enterkey(stin, buff, e, data))
			return (2);
	}
	else
		return (1);
	return (0);
}

static void	l_edit_3(char *buff, char **stin, t_data *data)
{
	int m;

	m = 0;
	if (buff[0] == 127)
	{
		if (data->cursor > 0)
		{
			*stin = delete_one(*stin, data);
			move_left(data);
			m = data->cursor;
			tputs(data->a->sc_s, 1, print);
			reset_line(data, *stin);
			data->cursor = m;
		}
	}
	else if (ft_isprint(buff[0]) && buff[1] == '\0')//maybe allow only printable
		writer(data, stin, buff);
}

char		*line_edit(t_data *data)
{
	t_edit *e;

	e = ft_memalloc(sizeof(t_edit));
	init_l_edit(e, data);
	while (42)  // need \n
	{
		act_pos(data);
		//if (e->stin)
		//		data->line_count = ((int)ft_strlen(e->stin) + data->start_col - 1) / data->scr_col;
		ft_bzero(e->buff, 6);
		read(0, e->buff, 5);
		//ft_printf("{%d-%d-%d-%d-%d}\n", e->buff[0], e->buff[1], e->buff[2], e->buff[3], e->buff[4]);ft_printf("%s\n", e->buff);
		if (l_edit_1(e->buff, &e->stin, data))
		{
			if ((e->ler = l_edit_2(e->buff, &e->stin, e, data)) || e->ler == 2)
			{
				if (e->ler == 2)
					return (end_line(data, e));
				l_edit_3(e->buff, &e->stin, data);
			}
		}
	}
	add_history(e->stin, data);
	//end_line(data, e->stin, e->buff);// utility ??
	return (end_line(data, e));
}
