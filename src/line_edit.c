/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 16:14:06 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/24 21:14:12 by kbagot           ###   ########.fr       */
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
	if (data->cursor== (int)ft_strlen(stin))
		stin[data->cursor - 1] = '\0';
	else
		stin = ft_strjoin(ft_strsub(stin, 0, data->cursor - 1), &stin[data->cursor]); //stin leaks
	//	printf("[%d] | [%s]", data->cursor, stin);
	return (stin);
}

static void	add_history(char *cmd, t_data *data)
{
	if (cmd && data->hist == NULL)
	{
		//		ft_printf("salut1\n");
		data->hist = ft_memalloc(sizeof(t_hist));
		data->hist->elem = ft_strdup(cmd);
		data->hist->next = NULL;
		data->hist->before = NULL;
	}
	else if (cmd && data->hist)
	{
		//	ft_printf("salut2\n");
		while (data->hist->next)
			data->hist = data->hist->next;
		data->hist->next = ft_memalloc(sizeof(t_hist));
		data->hist->next->elem = ft_strdup(cmd);
		data->hist->next->before = data->hist;
		data->hist->next->next = NULL;
		data->hist = data->hist->next;
	}
}

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
			break;
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
			break;
		tputs(tgetstr("nd", NULL), 1, print);
		data->cursor++;
	}
}

static void	move_by_word(t_data *data, char *stin, char *buff)
{
	if (buff[0] == 1 && stin && data->cursor > 0)
		word_left(data, stin);
	else if (buff[0] == 4 && stin)//alt +right  ctrl + D
		word_right(data, stin);
}

char		*line_edit(t_data *data)
{
	char	*buff;
	char	*stin;

	stin = NULL;
	buff = ft_strnew(4);
	tputs(tgetstr("im", NULL), 1, print);
	tputs(tgetstr("sc", NULL), 1, print);
	data->cursor = 0;
	while (1)  // need \n
	{
		ft_bzero(buff, 6);
		read(0, buff, 5);
	//	ft_printf("lol : %d-%d-%d-%d-%d\n", buff[0], buff[1], buff[2], buff[3], buff[4]);
		if (buff[0] == 27 && buff[1] == 91)//arrow
			arrow_key(data, &stin, buff);
		else if (buff[0] == 1 || buff[0] == 4)//alt +left  WORD MOVE ctrl + A
			move_by_word(data, stin, buff);
		//		else if (buff[0] == 3)//ctrl + d   now ctrl+ C
		//		{
		//			init_term(RESTORE);
		//			tputs(tgetstr("ei", NULL), 1, print);
		//			exit(0);
		//		}
		//ENTER CLIP MODE
		else if (buff[0] == 9 && buff[1] == 0)//copy mode //cut mode  CTRL + I
		{
			int ce;

			tputs(tgetstr("so", NULL), 1, print);
			ce = data->cursor;
			while (42)
			{
				ft_bzero(buff, 6);
				read(0, buff, 5);
				if (buff[0] == 27 && buff[1] == 91)//arrow
				{
					if (data->cursor >= 0 && data->cursor < (int)ft_strlen(stin))
					{
						tputs(tgetstr("ei", NULL), 1, print);
						ft_putchar(stin[data->cursor]);
						tputs(tgetstr("le", NULL), 1, print);
						tputs(tgetstr("im", NULL), 1, print);
					}
					arrow_key(data, &stin, buff);
				}
				else if (buff[0] == 1 || buff[0] == 4)//alt +left  WORD MOVE ctrl + A
					move_by_word(data, stin, buff);
				else if (buff[0] == 21 && buff[1] == 0)// copy  |   CTRL + U 
				{
					if (ce >= data->cursor)
						data->clipboard = ft_strsub(stin, data->cursor, ce);
					else if (ce < data->cursor)
						data->clipboard = ft_strsub(stin, ce, data->cursor);
					break;
				}
				else if (buff[0] == 11 && buff[1] == 0)// cut       |      CTRL + K
				{
					if (ce >= data->cursor)
					{
						data->clipboard = ft_strsub(stin, data->cursor, ce);
						stin = ft_strjoin(ft_strsub(stin, 0, data->cursor), &stin[ce]); //stin leaks
					}
					else if (ce < data->cursor)
					{
						data->clipboard = ft_strsub(stin, ce, data->cursor);
						stin = ft_strjoin(ft_strsub(stin, 0, ce), &stin[data->cursor + 1]); //stin leaks
					}
					break;
				}
				else if (buff[0] == 27 && buff[1] == 0)//echap
					break;
			}
			tputs(tgetstr("se", NULL), 1, print);
		}
		else if (buff[0] == 16 && buff[1] == 0)// PASTE   CTRL+P
		{
			ft_printf("%s", data->clipboard);
			stin = ft_strjoin(ft_strjoin(ft_strsub(stin, 0, data->cursor - 1), data->clipboard),
					&stin[data->cursor]); //stin leaks
			data->cursor += ft_strlen(data->clipboard);
		}
		else if (buff[0] == 12)
		{
			tputs(tgetstr("cl", NULL), 1, print);
			data->cursor = 0;
		}
		else if (buff[0] == 10) // send
		{
			tputs(tgetstr("do", NULL), 1, print);
			add_history(stin, data);
			tputs(tgetstr("ei", NULL), 1, print);
			ft_strdel(&buff);
			return (stin);
		}
		else if (buff[0] == 127)
		{		//	ft_printf("%c", buff[0]);
			if (data->cursor > 0)
			{
				// 		printf("salut\n");
				stin = delete_one(stin, data);
				tputs(tgetstr("le", NULL), 1, print);
				tputs(tgetstr("dc", NULL), 1, print);
				data->cursor--;
			}
		}
		else if (ft_isprint(buff[0]) && buff[1] == '\0')//maybe allow only printable
		{
			//	printf("l\n");
			if (stin == NULL)
				stin = ft_strdup(buff);
			else if (data->cursor == (int)ft_strlen(stin)) // maybe -1
				stin = ft_strjoin(stin, buff);  //stin leaks
			else
				stin = join(ft_strsub(stin, 0, data->cursor), buff, &stin[data->cursor]); //stin leaks
			if (buff[0] == 9)
				buff[0] = 32;
			ft_putchar(buff[0]);
			data->cursor++;
		}
		//	printf("%s\n", stin);
	}
	ft_strdel(&buff);
	tputs(tgetstr("ei", NULL), 1, print);
	return (stin);
}
