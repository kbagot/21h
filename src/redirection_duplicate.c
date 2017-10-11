/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_duplicate.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 17:24:43 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/10 17:28:40 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	print_error(int w)
{
	ft_putstr_fd("21sh: ", 2);
	ft_putnbr_fd(w, 2);
	ft_putstr_fd(": Bad file descriptor\n", 2);
	return (1);
}

int			dup_in_out_put(char **cmd, int i, int std_fd)
{
	int n;
	int w;
	int t;

	t = -1;
	if (i != std_fd)
		n = ft_atoi(cmd[0]);
	else
		n = std_fd;
	if (!ft_strcmp(cmd[i], "-"))
		close(n);
	if (ft_isnbr(cmd[i]))
	{
		w = ft_atoi(cmd[i]);
		if ((w >= 10 && w <= 12) || (t = dup(w)) == -1)
			return (print_error(w));
		if (t != -1)
			close(t);
		dup2(w, n);
	}
	return (0);
}
