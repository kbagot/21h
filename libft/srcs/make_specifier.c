/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_specifier.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <kbagot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 19:24:07 by kbagot            #+#    #+#             */
/*   Updated: 2017/02/13 20:13:02 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	make_specifier(va_list ap, t_prt *prt)
{
	int		n;
	t_flag	*flag;

	n = 0;
	prt->ispec = 0;
	flag = (t_flag*)malloc(sizeof(t_flag));
	init_sub_specif(flag, prt, ap);
	prt->i = prt->prt[ft_strlen(prt->prt) - 1];
	prt->spec = NULL;
	if (prt->i == '%')
	{
		prt->spec = ft_strnew(1);
		prt->spec[0] = '%';
		add_prt(prt, flag);
	}
	else if (ft_strchr("DduUixXpoO", prt->i))
	{
		prt->spec = ft_strnew(20);
		add_length(flag, prt, ap);
	}
	else
		make_char_specif(ap, prt, flag);
	free(flag);
}
