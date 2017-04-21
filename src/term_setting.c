/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_setting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 12:07:27 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/21 20:56:38 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_term(int act)
{
	int				ret;
	char			t_buff[1024];
	struct termios	term;

	if (act == 1)
	{
		ret = tgetent(t_buff, getenv("TERM"));
		tcgetattr(0, &term);
		term.c_lflag &= ~(ECHO | ICANON);
		term.c_cc[VTIME] = 0;
		term.c_cc[VMIN] = 1;
		tcsetattr(0, TCSADRAIN, &term);
	}
}
