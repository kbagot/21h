/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 17:54:21 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 17:56:46 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	parse_token_two(char **stin, int i)
{
	char	*c;

	if (((c = ft_strchr(stin[i], '&')) && (c[1] != '<' && c[1] != '>') &&
				(c - stin[i] > 0 && c[-1] != '<' && c[-1] != '>')) ||
			(!ft_strcmp(stin[i], "<<") && !stin[i + 1]))
	{
		ft_putstr_fd("21sh: syntax error near unexpected token", 2);
		if (!stin[i + 1])
			ft_putstr_fd(" `newline'\n", 2);
		else
			ft_putstr_fd(" `&'\n", 2);
		return (1);
	}
	else if ((!ft_strcmp(stin[i], "|") && !ft_strcmp(stin[i + 1], "|")) ||
			(!ft_strcmp(stin[i], "&")))
	{
		ft_putstr_fd("21sh: syntax error near unexpected token", 2);
		if (stin[i][0] == '|')
			ft_putstr_fd(" `|'\n", 2);
		else if (stin[i][0] == '&')
			ft_putstr_fd(" `&'\n", 2);
		return (1);
	}
	return (0);
}

static int	parse_token(char **stin)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (stin[j])
		j++;
	if (stin[0] && (strcmp(stin[0], "|") == 0 ||
				(j > 0 && strcmp(stin[j - 1], "|") == 0)))
	{
		ft_putstr_fd("21sh: error near unexpected token `|'\n", 2);
		return (1);
	}
	while (stin[i])
		if (parse_token_two(stin, i++))
			return (1);
	return (0);
}

int		parse_error(char **stin)
{//TOKEN
	int		i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (stin && stin[i])
	{
		tmp = strmsplit(stin[i], " \t\n");// obselete
		if (!tmp[0] && stin[i + 1])
		{
			ft_putstr_fd("21sh: syntax error near unexpected token `;'\n", 2);
			ft_tabdel(&tmp);
			return (0);
		}
		if (parse_token(tmp))
		{
			ft_tabdel(&tmp);
			return (0);
		}
		ft_tabdel(&tmp);
		i++;
	}
	return (1);
}
