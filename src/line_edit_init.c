/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 18:00:01 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 18:08:36 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	load_ansi(t_data *data)
{
	data->a->cm_s = tgetstr("cm", NULL);
	data->a->dl_s = tgetstr("dl", NULL);
	data->a->do_s = tgetstr("do", NULL);
	data->a->le_s = tgetstr("le", NULL);
	data->a->nd_s = tgetstr("nd", NULL);
	data->a->sc_s = tgetstr("sc", NULL);
	data->a->rc_s = tgetstr("rc", NULL);
	data->a->ku_s = tgetstr("ku", NULL);
	data->a->kd_s = tgetstr("kd", NULL);
	data->a->kr_s = tgetstr("kr", NULL);
	data->a->kl_s = tgetstr("kl", NULL);
	data->a->cl_s = tgetstr("cl", NULL);
	data->a->up_s = tgetstr("up", NULL);
	data->a->dc_s = tgetstr("dc", NULL);
	data->a->se_s = tgetstr("se", NULL);
	data->a->so_s = tgetstr("so", NULL);
}

void	init_l_edit(t_edit *e, t_data *data)
{
	e->ler = 0;
	e->stin = NULL;
	e->buff = ft_strnew(6);
	data->cursor = 0;
	init_term(data);
	load_ansi(data);
	tputs(data->a->sc_s, 1, print);
	act_pos(data);
	data->start_row = data->row;
	data->start_col = data->col;
}

char	*end_line(t_data *data, t_edit *e)
{
	char *ret;

	//tputs(tgetstr("ei", NULL), 1, print);
	ret = NULL;
	if (e->stin)
		ret = ft_strdup(e->stin);
	ft_strdel(&e->buff);
	ft_strdel(&e->stin);
	free(e);
	reset_term(data);
	return(ret);
}

void		act_pos(t_data *d)
{
	struct winsize	sz;
	char			*ansi;
	char			*buff;
	char			*tmp;

	ansi = ft_strnew(1);
	buff = ft_strnew(10);
	ioctl(0, TIOCGWINSZ, &sz);
	d->scr_row = sz.ws_row;
	d->scr_col = sz.ws_col;
	tputs("\E[6n", 1, print);
	while (ft_strchr(buff, 'R') == NULL)
	{
		ft_bzero(buff, 10);
		read(0, buff, 10);
		tmp = ansi;
		ansi = ft_strjoin(tmp, buff);
		ft_strdel(&tmp);
	}
	//ft_printf("[%s | %s]", &ansi[2], &ft_strchr(&ansi[2], ';')[1]);
	d->row = ft_atoi(&ansi[2]);
	d->col = ft_atoi(&ft_strchr(&ansi[2], ';')[1]);
	ft_strdel(&buff);
	ft_strdel(&ansi);
}
