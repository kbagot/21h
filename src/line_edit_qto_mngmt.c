/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_qto_mngmt.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:39:33 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/11 19:23:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		line_edit_qto_mngmt(char **stin, t_edit *e, t_data *data)
{
	if (*stin)
	{
		if (e->q_save)
			e->q_save = join(e->q_save, *stin, "\n");
		else
			e->q_save = ft_strdup(*stin);
		ft_strdel(&(*stin));
	}
	else if (e->q_save)
		e->q_save = join(e->q_save, "\n", "");
	data->cursor = 0;
	if (conform_quote(e->q_save) != 0)
	{
		if (!e->q_count)
			e->q_save = join(e->q_save, "\n", "");
		e->q_count = 1;
		ft_putstr("\n> ");
		return (0);
	}
	*stin = e->q_save;
	return (1);
}
