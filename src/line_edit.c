/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 16:14:06 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/26 18:56:13 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if (data->cursor== (int)ft_strlen(stin))
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

static void	end_line(t_data *data, char *stin, char *buff)
{
	tputs(tgetstr("do", NULL), 1, print);
	add_history(stin, data);
	tputs(tgetstr("ei", NULL), 1, print);
	ft_strdel(&buff);
	reset_term();
}

char		*line_edit(t_data *data)
{
	char	*buff;
	char	*stin;

	stin = NULL;
	buff = ft_strnew(6);
	tputs(tgetstr("im", NULL), 1, print);
	tputs(tgetstr("sc", NULL), 1, print);
	data->cursor = 0;
	init_term();
	while (42)  // need \n
	{
		ft_bzero(buff, 6);
		read(0, buff, 5);
		///	ft_printf("lol : %d-%d-%d-%d-%d\n", buff[0], buff[1], buff[2], buff[3], buff[4]);
		if (buff[0] == 27 && buff[1] == 91)//arrow
			arrow_key(data, &stin, buff);
		else if (buff[0] == 6 || buff[0] == 2)// ctrl + F  ctr+B
			move_by_word(data, stin, buff);
		else if (buff[0] == 4)//ctrl + d   now ctrl+ C  [make exit]
		{
			ft_strdel(&stin);
			stin = ft_strdup("exit");
			end_line(data, stin, buff);
			return (stin);
		}
		else if (buff[0] == 9 && buff[1] == 0)//copy mode //cut mode  CTRL + I
			copy_cut(data, &stin, buff);
		else if (buff[0] == 16 && buff[1] == 0)// PASTE   CTRL+P
			paste(data, &stin);
		else if (buff[0] == 12)  // CTRL+L CLEAR
		{
			tputs(tgetstr("cl", NULL), 1, print);
			data->cursor = 0;
		}
		else if (buff[0] == 10) // send
		{
			end_line(data, stin, buff);
			return (stin);
		}
		else if (buff[0] == 127)
		{
			if (data->cursor > 0)
			{
				stin = delete_one(stin, data);
				tputs(tgetstr("le", NULL), 1, print);
				tputs(tgetstr("dc", NULL), 1, print);
				data->cursor--;
			}
		}
		else if (ft_isprint(buff[0]) && buff[1] == '\0')//maybe allow only printable
			writer(data, &stin, buff);
	}
	ft_strdel(&buff);
	tputs(tgetstr("ei", NULL), 1, print);
	reset_term();
	return (stin);
}
