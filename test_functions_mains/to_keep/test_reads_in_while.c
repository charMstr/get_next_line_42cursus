/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:31:16 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/15 18:23:52 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

/*
** RETURN: new link matching the file descriptor.
*/

fd_gnl	*new_link(int fd)
{
	fd_gnl	*new_link;

	if (!(new_link = (fd_gnl*)malloc(sizeof(*new_link))))
		return (NULL);
	new_link->fd = fd;
	new_link->rest = NULL;
	new_link->next = NULL;
	return (new_link);
}

/*
** note: if add is 0 -> try to remove link containing the matching fd
**
** note2: if add is 1 -> RETURN link containing the matching fd, or created one
*/

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

void	fd_links_print(fd_gnl *list)
{
	int link;

	link = 0;
	while (list)
	{
		printf("in link: %d, fd is:%d\n", link, list->fd);
		if (list->rest)
			printf("string in fd(%d): \"%s\"\n", list->fd, list->rest);
		else
			printf("no string to be printed for this fd (%d)\n", list->fd);
		link++;
		list = list->next;
	}
	printf("\n");
}

/*
** note: 	1st set of conditions -> added feature:
**			if get_next_line() is called with NULL pointer it  will try to
**			remove the link corresponding to the given filedes.
**
** note2:	2nd set of conditions -> called only on the very frist time (static
**			pointer initiated to NULL).
**
** note:	in the code BUFFER_SIZE is replaced while compiling with -D option
*/

int		get_next_line(int fd, char **line)
{
	static fd_gnl	*head;
	fd_gnl			*fd_elem;

	if (!line && head)
		if (!manage_link(fd, &head, REMOVE))
			return (0);
	if (!head)
		if (!(head = new_link(fd)))
			return (0);
	fd_elem = manage_link(fd, &head, ADD);
	fd_links_print(head);
	return (0);
}

int	main(void)
{
	int		fd1 = 1;
	char	*line;
	int	bytes_read;

	line = NULL;
	if((fd1 = open("./file_lines2", O_RDONLY)) == -1)
		return (0);
	while (get_next_line(fd1, &line))
		printf("%s\n", line);
	printf("%s\n", line);
	if (!(line = (char*)malloc(sizeof(*line) * (BUFFER_SIZE + 1))))
		return (0);
	while ((bytes_read = read(fd1, line, BUFFER_SIZE)) > 0)
	{
		printf("bytes_read = %d\n", bytes_read);
		line[bytes_read] = '\0';
		printf("%s\n", line);
	}
/*
	bytes_read = read(fd1, line, BUFFER_SIZE);
	printf("bytes_read = %d\n", bytes_read);
	line[bytes_read] = '\0';
	printf("%s\n", line);
	bytes_read = read(fd1, line, BUFFER_SIZE);
	printf("bytes_read = %d\n", bytes_read);
	line[BUFFER_SIZE] = '\0';
	printf("%s\n", line);
*/

	get_next_line(fd1, NULL);
	free(line);
	close(fd1);
	return (0);
}
