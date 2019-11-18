/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:31:16 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/15 15:59:57 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

fd_gnl	*new_link(fd)
{
	fd_gnl	*new_link;
	if (!(new_link = (fd_gnl*)malloc(sizeof(*new_link))))
		return (NULL);
	new_link->fd = fd;
	new_link->rest = NULL;
	new_link->next = NULL;
	return (new_link);
}

fd_gnl	*manage_link(int fd, fd_gnl **list, int add)
{
	fd_gnl	**tracer;
	fd_gnl	*extra;

	if (!list || (!(*list) && !add))
		return (NULL);
	tracer = list;
	while (*tracer && ((*tracer)->fd != fd))
		tracer = &(*tracer)->next;
	if (*tracer && ((*tracer)->fd == fd) && add)
		return (*tracer);
	else if (*tracer && ((*tracer)->fd == fd) && !add)
	{
		extra = *tracer;
		*tracer = extra->next;
		if (!extra->rest)
			free(extra->rest);
		free(extra);
	}
	if (add)
		extra = (fd_gnl*)malloc(sizeof(*extra));
	if (!add || !extra)
		return (NULL);
	return (*tracer = new_link(fd));
}

void	find_print(fd_gnl *list)
{
	int link;

	link = 0;
	while (list)
	{
		printf("in link: %d, fd is:%d\n",link, list->fd);
		link++;
		list = list->next;
	}
	printf("\n");
}

int	main()
{
	int fd = 2;
	static fd_gnl *head;

	if (!head)
		if (!(head = new_link(fd)))
			return (0);
	printf("try to add fd to a unique list:\n");
	manage_link(1, &head, ADD);
	find_print(head);
	printf("try to add already existing fd:\n");
	manage_link(2, &head, ADD);
	find_print(head);
	manage_link(2, &head, REMOVE);
	printf("try to remove existing fd:\n");
	find_print(head);
	printf("try to remove last existing fd:\n");
	manage_link(1, &head, REMOVE);
	find_print(head);
	printf("try to readd a removed fdi to empty list:\n");
	manage_link(2, &head, ADD);
	find_print(head);
	printf("try to add fd to a unique list:\n");
	manage_link(1, &head, ADD);
	find_print(head);
	printf("try to add new fd to existing  list with 2 fd:\n");
	manage_link(3, &head, ADD);
	find_print(head);
	printf("try to add new fd o list with 3 fd:\n");
	manage_link(5, &head, ADD);
	find_print(head);
	printf("try to add new fd o list with 4 fd:\n");
	manage_link(7, &head, ADD);
	find_print(head);
	printf("try to remove very first fd link:\n");
	manage_link(2, &head, REMOVE);
	find_print(head);
	printf("try to remove very last fd link:\n");
	manage_link(7, &head, REMOVE);
	find_print(head);
	printf("try to remove non existing link:\n");
	manage_link(15, &head, REMOVE);
	find_print(head);
	printf("try to remove 'in the middle' fd link:\n");
	manage_link(3, &head, REMOVE);
	find_print(head);
	return (0);
}
