/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 14:22:38 by kbagot            #+#    #+#             */
/*   Updated: 2017/10/02 20:35:25 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	init_splt(t_split *stk, char const *s, char *c)
{
	stk->s = s;
	stk->c = c;
	stk->i = 0;
	stk->j = 0;
}

static int	stock_q(t_split *stk, char **split, char const *s)
{
	int		quote;

	quote = 0;
	quote = s[stk->i];
	if (s[stk->i] == '\"' || s[stk->i] == '\'')
	{
		split[stk->j][stk->k++] = s[stk->i];
		stk->i++;
		while (s[stk->i] && s[stk->i] != quote)
		{
			split[stk->j][stk->k++] = s[stk->i];
			stk->i++;
		}
		stk->i++;
		return (0);
	}
	return (1);
}

static void	stock_no_q(char **split, t_split *stk)
{
	char const	*s;
	char		*c;

	s = stk->s;
	c = stk->c;
	while (s[stk->i] && ft_strchr(c, s[stk->i]) == NULL)
	{
		if (s[stk->i] == '\'' || s[stk->i] == '\"')
			break ;
		split[stk->j][stk->k++] = s[stk->i];
		stk->i++;
	}
}

char		**splitforquote(char const *s, char *c)
{
	char	**split;
	t_split	*stk;

	if (s == NULL || (split = malloc(sizeof(char*) * (f_m(s, c) + 1))) == NULL)
		return (NULL);
	stk = ft_memalloc(sizeof(t_split));
	init_splt(stk, s, c);
	while (s[stk->i])
	{
		stk->k = 0;
		while (s[stk->i] && ft_strchr(c, s[stk->i]))
			stk->i++;
		if (s[stk->i])
		{
			if ((split[stk->j] = malloc(sizeof(char) *
							(f_l(s, stk->i, c) + 2))) == NULL)
				return (NULL);
			if (stock_q(stk, split, s))
				stock_no_q(split, stk);
			split[stk->j++][stk->k++] = '\0';
		}
	}
	split[stk->j] = NULL;
	free(stk);
	return (split);
}

/*
   int main(int ac, char **av)
   {
   char **lol;if (ac == 2){	lol = strquotesplit(av[1], ";");if(lol)while(*lol){printf("[%s]\n", *lol);lol++;}}return (0);
   }*/
