/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 15:41:47 by kbagot            #+#    #+#             */
/*   Updated: 2017/04/29 19:45:29 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	paste(t_data *data, char **stin)
{
	char *tmp;

	if (data->clipboard)
	{
		tmp = *stin;
		ft_printf("%s", data->clipboard);
		*stin = join(ft_strsub(tmp, 0, data->cursor), data->clipboard,
				&tmp[data->cursor]); //stin leaks
		data->cursor += ft_strlen(data->clipboard);
		ft_strdel(&tmp);
	}
}

void	writer(t_data *data, char **stin, char *buff)
{
	char *tmp;

	tmp = *stin;
	if (*stin == NULL)
		*stin = ft_strdup(buff);
	else if (data->cursor == (int)ft_strlen(*stin)) // maybe -1
		*stin = ft_strjoin(tmp, buff);
	else
		*stin = join(ft_strsub(tmp, 0, data->cursor), buff, &tmp[data->cursor]);
	if (buff[0] == 9)
		buff[0] = 32;
	ft_putchar(buff[0]);
	data->cursor++;
	ft_strdel(&tmp);
}
