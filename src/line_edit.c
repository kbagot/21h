/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 14:37:42 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/21 20:53:03 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int	print(int c)
{
	ft_putchar(c);
	return (0);
}

static char	*delete_one(char *stin, t_data *data)
{
	char *tmp;
	char *old;

	old = stin;
	if (data->cursor == (int)ft_strlen(stin))
		stin[data->cursor - 1] = '\0';
	else
	{
		tmp = ft_strsub(stin, 0, data->cursor - 1);
		stin = ft_strjoin(tmp, &stin[data->cursor]); //stin leaks
		ft_strdel(&old);
		ft_strdel(&tmp);
	}
	return (stin);
}

static void	add_history(char *cmd, t_data *data)
{
	if (cmd && data->hist == NULL)
	{
		data->hist = ft_memalloc(sizeof(t_hist));
		data->hist->elem = ft_strdup(cmd);
		data->hist->next = NULL;
		data->hist->before = NULL;
	}
	else if (cmd && data->hist)
	{
		while (data->hist->next)
			data->hist = data->hist->next;
		data->hist->next = ft_memalloc(sizeof(t_hist));
		data->hist->next->elem = ft_strdup(cmd);
		data->hist->next->before = data->hist;
		data->hist->next->next = NULL;
		data->hist = data->hist->next;
	}
}

static void	make_conform(char **stin, int *i, int c, char *nst)
{
	char	*tmp;
	int		l;

	tmp = *stin;
	*stin = join(ft_strsub(tmp, 0, *i), nst, &tmp[*i + c]);
	ft_strdel(&tmp);
	*i += ft_strlen(nst) - 1;
	l = *i + 1;
	tmp = *stin;
	while (stin[0][l] && stin[0][l] != ' ')
	{
		if (stin[0][l] == '<' || stin[0][l] == '>')
		{
			*stin = join(ft_strsub(tmp, 0, l), " ", &tmp[l]);
			ft_strdel(&tmp);
			break ;
		}
		l++;
	}
}

static char	*conform(char *stin)
{//maybe some shit in it
	int		i;

	i = 0;
	while (stin && stin[i])
	{// add a if in quote or not
		if (stin[i] == '|')
			make_conform(&stin, &i, 1, " | ");
		else if (!ft_strncmp(&stin[i], ">>", 2))
			make_conform(&stin, &i, 2, ">> ");
		else if (!ft_strncmp(&stin[i], "<<-", 3))
			make_conform(&stin, &i, 3, "<<- ");
		else if (!ft_strncmp(&stin[i], "<<", 2))
			make_conform(&stin, &i, 2, "<< ");
		else if (!ft_strncmp(&stin[i], ">", 1) && ft_strncmp(&stin[i], ">&", 2))
			make_conform(&stin, &i, 1, "> ");
		else if (!ft_strncmp(&stin[i], ">&", 2))
			make_conform(&stin, &i, 2, ">& ");
		else if (!ft_strncmp(&stin[i], "<", 1) && ft_strncmp(&stin[i], "<&", 2))
			make_conform(&stin, &i, 1, "< ");
		else if (!ft_strncmp(&stin[i], "<&", 1))
			make_conform(&stin, &i, 2, "<& ");
		i++;
	}
	return (stin);
}

static int	conform_quote(char *s)
{
	int i;
	int quote;

	quote = 0;
	i = 0;
	while (s && s[i])
	{
		if (quote == 0 && (s[i] == '\'' || s[i] == '\"'))
			quote = s[i];
		else if (quote != 0 && s[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
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
	d->row = ft_atoi(&ansi[2]);
	d->col = ft_atoi(&ft_strchr(&ansi[2], ';')[1]);
	ft_strdel(&buff);
	ft_strdel(&ansi);
}

static void	paste(t_data *data, char **stin)
{
	if (data->clipboard)
		writer(data, stin, data->clipboard);
}

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

static void	init_l_edit(t_edit *e, t_data *data)
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

static char	*end_line(t_data *data, t_edit *e)
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
