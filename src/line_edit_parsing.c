/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 18:08:49 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/11 17:00:41 by kbagot           ###   ########.fr       */
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

static void	conform_ctn(char **stin, int *i)
{
	if (!ft_strncmp(&stin[0][*i], ">&", 2))
		make_conform(stin, i, 2, ">& ");
	else if (!ft_strncmp(&stin[0][*i], "<", 1) &&
			ft_strncmp(&stin[0][*i], "<&", 2))
		make_conform(stin, i, 1, "< ");
	else if (!ft_strncmp(&stin[0][*i], "<&", 2))
		make_conform(stin, i, 2, "<& ");
	else if (!ft_strncmp(&stin[0][*i], ";", 1))
		make_conform(stin, i, 1, "; ");
}

static void	skip_quote(char *stin, int *i)
{
	int		quote;

	if (stin && stin[*i] && (stin[*i] == '\'' || stin[*i] == '\"'))
	{
		quote = *i;
		*i += 1;
		while (stin[*i] && stin[*i] != stin[quote])
			*i += 1;
	}
}

char		*conform(char *stin)
{
	int		i;

	i = 0;
	skip_quote(stin, &i);
	while (stin && stin[i])
	{
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
		conform_ctn(&stin, &i);
		i++;
		skip_quote(stin, &i);
	}
	return (stin);
}

int			conform_quote(char *s)
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
