/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 16:14:06 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/20 20:30:21 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	putshit(int c)
{
	ft_putchar(c);
	return (0);
}

static char	*delete_one(char *stin, int cursor)
{
	if (cursor == 0)
		stin = ft_strdup(&stin[1]);
	else if (cursor == (int)ft_strlen(stin) - 1)
		stin[cursor] = '\0';
	else
		stin = ft_strjoin(ft_strsub(stin, 0, cursor), &stin[cursor + 1]); //stin leaks
//	printf("[%d] | [%s]", cursor, stin);
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

char		*termcap(t_data *data)
{
	char	*buff;
	int		cursor;
	char	*stin;

	stin = NULL;
	buff = ft_strnew(4);
	tputs(tgetstr("im", NULL), 1, putshit);
	tputs(tgetstr("sc", NULL), 1, putshit);
	cursor = 0;
	while (1)  // need \n
	{
		ft_bzero(buff, 4);
		read(0, buff, 3);
//	ft_printf("%d-%d-%d\n", buff[0], buff[1], buff[2]);
		if (buff[0] == 27 && buff[1] == 91)//arrow
		{
//			printf("d[%s]  g[%s]\n", tgetstr("kr", NULL), tgetstr("kl", NULL));
//			printf("%s  {%s}\n", &buff[2], ap);
//			ft_printf("%c| %c\n", ntm[2], buff[2]);
//			HISTORY
			if (buff[2] && buff[2] == tgetstr("ku", NULL)[2] && data->hist) //UP
			{
				if (data->hist->before)
				{
					stin = ft_strdup(data->hist->before->elem);
					tputs(tgetstr("rc", NULL), 1, putshit);
					tputs(tgetstr("ce", NULL), 1, putshit);
					ft_printf("%s", stin);
					cursor = ft_strlen(stin);
					data->hist = data->hist->before;
				}
			}
			else if (buff[2] && buff[2] == tgetstr("kd", NULL)[2] && data->hist) //DOWN
			{
				if (data->hist->next)
				{
					stin = ft_strdup(data->hist->next->elem);
					tputs(tgetstr("rc", NULL), 1, putshit);
					tputs(tgetstr("ce", NULL), 1, putshit);
					ft_printf("%s", stin);
					cursor = ft_strlen(stin);
					data->hist = data->hist->next;
				}
			}
			else if (buff[2] && buff[2] == tgetstr("kr", NULL)[2] && stin && cursor < (int)ft_strlen(stin))
			{
				tputs(tgetstr("nd", NULL), 1, putshit);
				cursor++;
			}
			else if (buff[2] && buff[2] == tgetstr("kl", NULL)[2] && cursor > 0)
			{
				tputs(tgetstr("le", NULL), 1, putshit);
				cursor--;
			}
			else if (buff[2] && buff[2] == 72 && cursor != 0) //HOME
			{
					tputs(tgetstr("rc", NULL), 1, putshit);
					cursor = 0;
			}
			else if (buff[2] && buff[2] == 70 && stin) //END
			{
				while (cursor < (int)ft_strlen(stin))
				{
					tputs(tgetstr("nd", NULL), 1, putshit);
					cursor++;
				}
			}	
		}
		else if (buff[0] == 1)//alt +left  WORD MOVE ctrl + A
		{
			if (stin)
			{
				if (stin[cursor] == 9 || stin[cursor] == 32)
				{
					while (cursor > 0 && (stin[cursor] == 9 || stin[cursor] == 32))
					{
						tputs(tgetstr("le", NULL), 1, putshit);
						cursor--;
					}
				}
				else if (stin[cursor] != 9 || stin[cursor] != 32)
				{
					while (cursor > 0 && (stin[cursor] != 9 && stin[cursor] != 32))
					{
						tputs(tgetstr("le", NULL), 1, putshit);
						cursor--;
					}
				}
			}
		}
		else if (buff[0] == 4)//alt +right  ctrl + D
		{
			if (stin)
			{
				if (stin[cursor] == 9 || stin[cursor] == 32)
				{
					while (cursor < (int)ft_strlen(stin) && (stin[cursor] == 9 || stin[cursor] == 32))
					{
						tputs(tgetstr("nd", NULL), 1, putshit);
						cursor++;
					}
				}
				else if (stin[cursor] != 9 || stin[cursor] != 32)
				{
					while (cursor < (int)ft_strlen(stin) && (stin[cursor] != 9 && stin[cursor] != 32))
					{
						tputs(tgetstr("nd", NULL), 1, putshit);
						cursor++;
					}
				}
			}
		}
		else if (buff[0] == 3)//ctrl + d   now ctrl+ C
		{
			init_term(RESTORE);
			tputs(tgetstr("ei", NULL), 1, putshit);
			exit(0);
		}
		else if (buff[0] == 12)
		{
			tputs(tgetstr("cl", NULL), 1, putshit);
			cursor = 0;
		}
		else if (buff[0] == 10) // send
		{
			tputs(tgetstr("do", NULL), 1, putshit);
			add_history(stin, data);
			tputs(tgetstr("ei", NULL), 1, putshit);
			ft_strdel(&buff);
			return (stin);
		}
		else if (buff[0] == 127)
		{		//	ft_printf("%c", buff[0]);
			if (cursor > 0)
			{
		// 		printf("salut\n");
				stin = delete_one(stin, cursor - 1);
				tputs(tgetstr("le", NULL), 1, putshit);
				tputs(tgetstr("dc", NULL), 1, putshit);
				cursor--;
			}
		}
		else if (buff[1] == '\0')//maybe allow only printable
		{
		//	printf("l\n");
			if (stin == NULL)
				stin = ft_strdup(buff);
			else if (cursor == (int)ft_strlen(stin)) // maybe -1
				stin = ft_strjoin(stin, buff);  //stin leaks
			else
				stin = join(ft_strsub(stin, 0, cursor), buff, &stin[cursor]); //stin leaks
			if (buff[0] == 9)
				buff[0] = 32;
			ft_putchar(buff[0]);
			cursor++;
		}
	}
	ft_strdel(&buff);
	tputs(tgetstr("ei", NULL), 1, putshit);
	return (stin);
}
