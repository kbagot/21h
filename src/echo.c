/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/24 17:31:43 by kbagot            #+#    #+#             */
/*   Updated: 2017/03/30 16:05:29 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_quote(char *stin)
{
	int i;
	int cot;

	cot = 0;
	i = 0;
	while (stin[i])
	{
		if (stin[i] == 39 || stin[i] == 34)
			cot++;
		if (!stin[i])
			break ;
		i++;
	}
	if (cot % 2 != 0)
	{
		ft_putstr_fd("quote \n", 2);
		return (1);
	}
	return (0);
}

static int	cot_mod(char *stin, int cot)
{
	if (stin[0] == stin[1])
		return (0);
	if (cot == 0)
		return (1);
	else
		return (0);
}

int			exec_echo(char *stin)
{
	int cot;
	int i;

	i = -1;
	cot = 0;
	if ((check_quote(stin)) == 1)
		return (1);
	while (stin[i++])
	{
		if (stin[i] && (stin[i] == '\'' || stin[i] == '\"'))
			cot = cot_mod(&stin[i], cot);
		if (stin[i] && cot == 0 && stin[i] == ' ')
		{
			ft_putchar(' ');
			while (stin[i] && stin[i] == ' ')
				i++;
		}
		if (stin[i] && (stin[i] != '\'' && stin[i] != '\"'))
			ft_putchar(stin[i]);
	}
	ft_putchar('\n');
	return (1);
}
