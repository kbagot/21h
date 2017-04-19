/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 16:14:06 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/19 18:22:49 by kbagot           ###   ########.fr       */
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
		stin = ft_strjoin(&stin[0], &stin[cursor]);
//	printf("[%d] | [%s]", cursor, stin);
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
	else if (cmd)
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

char	*termcap(char *stin, t_data *data)
{
	char	*buff;
	char	*ap;
	int		cursor;

	ap = ft_strnew(1000); // a degager NULL power
	buff = ft_strnew(4);
	tputs(tgetstr("im", &ap), 1, putshit);
	tputs(tgetstr("sc", &ap), 1, putshit);
	cursor = 0;
	while (1)  // need \n
	{
		ft_bzero(buff, 4);
		read(0, buff, 3);

//		ft_printf("%d-%d-%d\n", buff[0], buff[1], buff[2]);
		if (buff[0] == 27 && buff[1] == 91)//arrow
		{
//			printf("d[%s]  g[%s]\n", tgetstr("kr", &ap), tgetstr("kl", &ap));
//			printf("%s  {%s}\n", &buff[2], ap);
//			ft_printf("%c| %c\n", ntm[2], buff[2]);
//			HISTORY
			if (buff[2] && buff[2] == tgetstr("ku", &ap)[2] && data->hist) //UP
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
			else if (buff[2] && buff[2] == tgetstr("kd", &ap)[2] && data->hist) //DOWN
			{
				if (data->hist->next)
				{
					stin = ft_strdup(data->hist->next->elem);
					tputs(tgetstr("rc", &ap), 1, putshit);
					tputs(tgetstr("ce", &ap), 1, putshit);
					ft_printf("%s", stin);
					cursor = ft_strlen(stin);
					data->hist = data->hist->next;
				}
			}
			else if (buff[2] && buff[2] == tgetstr("kr", &ap)[2] && stin && cursor < (int)ft_strlen(stin))
			{
				tputs(tgetstr("nd", &ap), 1, putshit);
				cursor++;
			}
			else if (buff[2] && buff[2] == tgetstr("kl", &ap)[2] && cursor > 0)
			{
				tputs(tgetstr("le", &ap), 1, putshit);
				cursor--;
			}
		}
		else if (buff[0] == 4)//ctrl + D 
		{
			init_term(RESTORE);
			exit(0);
		}
		else if (buff[0] == 12)
		{
			tputs(tgetstr("cl", &ap), 1, putshit);
			cursor = 0;
		}
		else if (buff[0] == 10) // send
		{
			tputs(tgetstr("do", &ap), 1, putshit);
			add_history(stin, data);
			return (stin);
		}
		else if (buff[0] == 127)
		{		//	ft_printf("%c", buff[0]);
			if (cursor > 0)
			{
		// 		printf("salut\n");
				stin = delete_one(stin, cursor - 1);
				tputs(tgetstr("le", &ap), 1, putshit);
				tputs(tgetstr("dc", &ap), 1, putshit);
				cursor--;
			}
		}
		else
		{
		//	printf("l\n");
			if (buff[0] == 9)
				buff[0] = 32;
			if (stin == NULL)
				stin = ft_strdup(buff);
			else	
				stin = ft_strjoin(stin, buff);
			ft_putchar(buff[0]);
			cursor++;
		}
	}
	return (stin);
}
