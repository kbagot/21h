/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_setting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 12:07:27 by kbagot            #+#    #+#             */
/*   Updated: 2017/06/09 20:36:47 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	init_term(t_data *d)
{
	int				ret;
	char			t_buff[1024];
	struct termios	term;
	char			*term_name;

	if ((term_name = getenv("TERM")) == NULL)
		term_name = "xterm-256color";
	ret = tgetent(t_buff, term_name);
	tcgetattr(0, &term);
	d->stored_settings = term;
	term.c_lflag &= ~(ECHO | ICANON);
	term.c_cc[VTIME] = 0;
	term.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &term);
}

void	reset_term(t_data *d)
{
	tcsetattr(0, TCSANOW, &d->stored_settings);
}
