/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/03 16:28:14 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/05 15:38:55 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int	exec_exit(char **cstin, t_data *data, char *stin)
{
	int s;

	if (cstin[1])
	{
		if (cstin[2])
		{
			ft_putstr_fd("exit: Too many arguments\n", 2);
			data->rvalue = 0;
			return (1);
		}
		else if (!ft_isdigit(cstin[1][0]))
		{
			ft_putstr_fd("exit: Syntax\n", 2);
			data->rvalue = 1;
		}
		else
			data->rvalue = ft_atoi(cstin[1]);
	}
	ft_strdel(&stin);
	s = data->rvalue;
	exit(s);
	return (1);
}
