/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbagot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 19:11:16 by kbagot            #+#    #+#             */
/*   Updated: 2016/11/19 15:06:34 by kbagot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *lstnew;
	t_list *dlstnew;

	if (lst == NULL)
		return (NULL);
	lstnew = f(lst);
	dlstnew = lstnew;
	while (lst->next)
	{
		lstnew->next = f(lst->next);
		if (lstnew->next == NULL)
		{
			free(dlstnew);
			return (NULL);
		}
		lst = lst->next;
		lstnew = lstnew->next;
	}
	return (dlstnew);
}
