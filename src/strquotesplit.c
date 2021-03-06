/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 18:35:32 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/06 22:20:22 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	fl(char const *str, int i, char *c)
{
	int lettre;
	int quote;

	quote = 0;
	lettre = 0;
	while (str[i] && (quote != 0 || ft_strchr(c, str[i]) == NULL))
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
		lettre++;
	}
	return (lettre);
}

static int	fm(char const *str, char *c)
{
	int i;
	int mot;
	int quote;

	quote = 0;
	mot = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(c, str[i]))
			i++;
		if (str[i])
		{
			while (str[i] && (quote != 0 || ft_strchr(c, str[i]) == NULL))
			{
				if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
					quote = str[i];
				else if (str[i] == quote)
					quote = 0;
				i++;
			}
			mot++;
		}
	}
	return (mot);
}

static void	stock(t_split *stk, char const *s, char *c, char **split)
{
	while (s[stk->i] && (stk->quote != 0 || ft_strchr(c, s[stk->i]) == NULL))
	{
		if ((s[stk->i] == '\'' || s[stk->i] == '\"') && stk->quote == 0)
			stk->quote = s[stk->i];
		else if (s[stk->i] == stk->quote)
			stk->quote = 0;
		split[stk->j][stk->k++] = s[stk->i++];
	}
}

static void	init_splt(t_split *stk, char const *s, char *c)
{
	stk->s = s;
	stk->c = c;
	stk->i = 0;
	stk->j = 0;
	stk->quote = 0;
}

char		**strquotesplit(char const *s, char *c)
{
	t_split	*stk;
	char	**split;

	if (s == NULL || (split = malloc(sizeof(char*) * (fm(s, c) + 1))) == NULL)
		return (NULL);
	stk = ft_memalloc(sizeof(t_split));
	init_splt(stk, s, c);
	while (s[stk->i])
	{
		while (s[stk->i] && ft_strchr(c, s[stk->i]))
			stk->i++;
		if (s[stk->i])
		{
			stk->k = 0;
			stk->quote = 0;
			if ((split[stk->j] = malloc(sizeof(char) *
							(fl(s, stk->i, c) + 1))) == NULL)
				return (NULL);
			stock(stk, s, c, split);
			split[stk->j++][stk->k++] = '\0';
		}
	}
	split[stk->j] = NULL;
	free(stk);
	return (split);
}
