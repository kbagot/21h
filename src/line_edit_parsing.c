/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 18:08:49 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 20:09:44 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	make_conform(char **stin, int *i, int c, char *nst)
{
	char	*tmp;
	int		l;

	tmp = *stin;
	*stin = join(ft_strsub(tmp, 0, *i), nst, &tmp[*i + c]);
	ft_strdel(&tmp);
	*i += ft_strlen(nst) - 1;
	l = *i + 1;
	tmp = *stin;
	while (stin[0][l] && stin[0][l] != ' ')
	{
		if (stin[0][l] == '<' || stin[0][l] == '>')
		{
			*stin = join(ft_strsub(tmp, 0, l), " ", &tmp[l]);
			ft_strdel(&tmp);
			break ;
		}
		l++;
	}
}

char	*conform(char *stin)
{//maybe some shit in it
	int		i;

	i = 0;
	while (stin && stin[i])
	{// add a if in quote or not
		if (stin[i] == '|')
			make_conform(&stin, &i, 1, " | ");
		else if (!ft_strncmp(&stin[i], ">>", 2))
			make_conform(&stin, &i, 2, ">> ");
		else if (!ft_strncmp(&stin[i], "<<-", 3))
			make_conform(&stin, &i, 3, "<<- ");
		else if (!ft_strncmp(&stin[i], "<<", 2))
			make_conform(&stin, &i, 2, "<< ");
		else if (!ft_strncmp(&stin[i], ">", 1) && ft_strncmp(&stin[i], ">&", 2))
			make_conform(&stin, &i, 1, "> ");
		else if (!ft_strncmp(&stin[i], ">&", 2))
			make_conform(&stin, &i, 2, ">& ");
		else if (!ft_strncmp(&stin[i], "<", 1) && ft_strncmp(&stin[i], "<&", 2))
			make_conform(&stin, &i, 1, "< ");
		else if (!ft_strncmp(&stin[i], "<&", 2))
			make_conform(&stin, &i, 2, "<& ");
		else if (!ft_strncmp(&stin[i], ";", 1))
			make_conform(&stin, &i, 1, "; ");
		i++;
	}
	return (stin);
}

int	conform_quote(char *s)
{
	int i;
	int quote;

	quote = 0;
	i = 0;
	while (s && s[i])
	{
		if (quote == 0 && (s[i] == '\'' || s[i] == '\"'))
			quote = s[i];
		else if (quote != 0 && s[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
}

