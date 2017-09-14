/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strquotesplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 14:22:38 by kbagot            #+#    #+#             */
/*   Updated: 2017/09/14 14:26:19 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	f_l(char const *str, int i, char *c)
{
	int letter;
	int quote;

	letter = 0;
	quote = 0;
	if (str[i] == '\"' || str[i] == '\'')
	{
		quote = str[i];
		i++;
		while (str[i] && str[i] != quote)
		{
			i++;
			letter++;
		}
		i++;
	}
	else
		while (str[i] && (ft_strchr(c, str[i]) == NULL))
		{
			i++;
			letter++;
			if (str[i] == '\'' || str[i] == '\"')
				break ;
		}
	return (letter);
}

static void	f_mnorm(int *quote, int *i, int *mot, char const *str)
{
	*quote = str[*i];
	*i += 1;
	while (str[*i] && str[*i] != *quote)
		*i += 1;
	*i += 1;
	*mot += 1;
}

static int	f_m(char const *str, char *c)
{
	int i;
	int mot;
	int	quote;

	mot = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(c, str[i]))
			i++;
		quote = 0;
		if (str[i] && (str[i] == '\"' || str[i] == '\''))
			f_mnorm(&quote, &i, &mot, str);
		else if (str[i])
		{
			while (str[i] && (ft_strchr(c, str[i]) == NULL))
			{
				i++;
				if (str[i] == '\'' || str[i] == '\"')
					break ;
			}
			mot++;
		}
	}
	return (mot);
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

static void	init_splt(t_split *stk, char const *s, char *c)
{
	stk->s = s;
	stk->c = c;
	stk->i = 0;
	stk->j = 0;
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
							(f_l(s, stk->i, c) + 1))) == NULL)
				return (NULL);
			if (stock_q(stk, split, s))
				stock_no_q(split, stk);
			split[stk->j++][stk->k++] = '\0';
		}
	}
	split[stk->j] = NULL;
	return (split);
}

/*
   int main(int ac, char **av)
   {
   char **lol;if (ac == 2){	lol = strquotesplit(av[1], ";");if(lol)while(*lol){printf("[%s]\n", *lol);lol++;}}return (0);
   }*/
