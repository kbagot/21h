/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 14:37:42 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/11 19:23:32 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	l_edit_1(char *buff, char **stin, t_data *data)
{
	if (buff[0] == 27 && buff[1] == 91)
		arrow_key(data, stin, buff);
	else if (buff[0] == 6 || buff[0] == 2 || buff[0] == 19 || buff[0] == 24)
		move_by_word_line(data, *stin, buff);
	else if ((buff[0] == 25 || buff[0] == 11) && buff[1] == 0)
		copy_cut(data, stin, buff);
	else if (buff[0] == 16 && buff[1] == 0)
		paste(data, stin);
	else
		return (1);
	return (0);
}

static int	l_edit_2_enterkey(char **stin, char *buff, t_edit *e, t_data *data)
{
	if (*stin)
		go_end(data, *stin);
	if (get_proc(0) == 1)
	{
		ft_strdel(stin);
		get_proc(2);
	}
	if (buff[0] == 10 || buff[0] == 12)
	{
		if (buff[0] == 12 || line_edit_qto_mngmt(&(*stin), e, data))
		{
			if (buff[0] == 12)
			{
				tputs(data->a->cl_s, 1, print);
				ft_strdel(&(*stin));
			}
			else if (buff[0] != 12)
				ft_putchar('\n');
			add_history(e->stin, data);
			*stin = conform(*stin);
			return (1);
		}
	}
	return (0);
}

static int	l_edit_2(char *buff, char **stin, t_edit *e, t_data *data)
{
	if ((!stin || (stin && stin[0] == '\0')) && buff[0] == 4)
	{
		ft_strdel(stin);
		*stin = ft_strdup("exit");
		return (2);
	}
	else if (buff[0] == 10 || buff[0] == 12)
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
	else if (ft_isprint(buff[0]) && buff[1] == '\0')
		writer(data, stin, buff);
}

char		*line_edit(t_data *data)
{
	t_edit *e;

	e = ft_memalloc(sizeof(t_edit));
	init_l_edit(e, data);
	while (42)
	{
		act_pos(data);
		ft_bzero(e->buff, 7);
		read(0, e->buff, 6);
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
	return (end_line(data, e));
}
