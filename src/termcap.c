/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 16:14:06 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/18 20:16:47 by kbagot           ###   ########.fr       */
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
	else if (cursor == (int)ft_strlen(stin))
		stin[cursor] = '\0';
	else
		stin = ft_strjoin(&stin[0], &stin[cursor]);
	return (stin);
}

char	*termcap(char *stin)
{
	char	*buff;
	char	*ap;
	int		cursor;

	ap = ft_strnew(50);
	buff = ft_strnew(4);
	tputs(tgetstr("im", &ap), 1, putshit);
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
			if (buff[2] && buff[2] == tgetstr("kr", &ap)[2] && stin && cursor < (int)ft_strlen(stin))
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
			exit(0);
		else if (buff[0] == 12)
		{
			tputs(tgetstr("cl", &ap), 1, putshit);
			cursor = 0;
		}
		else if (buff[0] == 10) // send
		{
			tputs(tgetstr("do", &ap), 1, putshit);
			return (stin);
		}
		else if (buff[0] == 127)
		{		//	ft_printf("%c", buff[0]);
			if (cursor > 0)
			{
				stin = delete_one(stin, cursor - 1);
				tputs(tgetstr("le", &ap), 1, putshit);
				tputs(tgetstr("dc", &ap), 1, putshit);
				cursor--;
			}
		}
		else
		{
		//	printf("l\n");
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
