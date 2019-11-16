/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:31:16 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/16 01:19:06 by charmstr         ###   ########.fr       */
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
	new_link->len_rest = 0;
	new_link->len_line = 0;
	new_link->eof = 0;
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
		if (!extra->rest):
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
** note3:	in the code, BUFFER_SIZE is replaced while compiling with -D option
**
** RETURN: 1:line was read, 0: EOF, -1:problem occured
**
** if RETURN is -1. should we free the fd? or the whole lists of fd.
*/

int		get_next_line(int fd, char **line)
{
	static fd_gnl	*head;
	fd_gnl			*fd_link;
	int				result;

	if (!line)
		if (!manage_link(fd, &head, REMOVE))
			return (0);// pourrait etre -1, ne change rien pour moi, dependra de la philosophie de la norminette...
	if (!head)
		if (!(head = new_link(fd)))
			return (-1);
	fd_link = manage_link(fd, &head, ADD);
	result = to_read_or_not_to_read(fd_link, line);
	if (!result)
	{
		manage_link(fd, &head, REMOVE);
		return (result);
	}
	else return (result);
	//fd_links_print(head);
}

/*
** man:	should read  until a '\n' or EOF is reached, then update line so
**			that it can be printed outside of this get_next_line call.
**
** note:	whatever followed the first '\n' is stored  into fd->rest
**
** RETURN:	-1 if a problem occured
**			0 if (fd->eof == 1 and fd->len == 0)
**			1 if (fd->eof = 0 || fd->len != 0)
*/

int	to_read_or_not_to_read(fd_gnl *fd_link, char **line)
{
	int		res;
	int		bytes_r;
	char	buf[BUFFER_SIZE + 1];
	int		update_result;

	buff[BUFFER_SIZE] = '\0';
	if (fd_link->len_rest)
		update_result = update_line(line, fd_link->rest, fd_link);
	while ((bytes_r = read(fd1, buff, BUFFER_SIZE)) > 0 && update_result > 0)
	{
		buf[bytes_r] = '\0';
		if (bytes_r < BUFFER_SIZ)
			fd_link->eof = 1;
		update_result = update_line(line, buff, fd_link);
	}
	if (bytes_r == -1 || update_result == -1)
		return (-1);
	if (fd_link->eof && (fd_link->len_rest == 0))
		return (0);
	return (1);
}

/*
** man: concatenate anything before a '\n' in string with existing line.
**		place the rest in fd_link->rest;
**
** RETURN:	1 if no '\n' was found,
**			1 if one was found at the very end of buff and fd_link->eof == 0
**
**			0 if a '\n' was found before the end of rest
**			0 if a '\n' is found at the end of rest and fd_link->eof == 1
**
**			-1 if problem occured.
*/

int		update_line(char **line, char *parse_me, fd_gnl *link)
{
	char	*tmp;
	int		i;
	int		j;

	j = -1;
	i = 0;
	tmp = *line;
	while (parse_me[i] && parse_me[i] != '\n')
		i++;
	if (i) //STOP in this function. will need to do a sixth function. or a seventh thats it
	{
		if (!(*line = (char *)malloc(sizeof(char) * (link->len_line + i + 1))))
			return (-1);
		while (++j < link->len_line)
			line[j] = tmp[j];
		j = -1;
		while (++j < i - 1)
			line[link->len_line + j] = parse_me[j];
	}
	link->len_line = link->len_line + i;
	link>rest = whats left. faire un dup de link->len_rest - la positiondu '\n' a detruire
	link->len_rest = ...
	if (tmp)
		free(tmp);
	return ();
}

int		find_bn(char *str)
{
	int i;

	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	main(void)
{
	int		fd1;
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
	get_next_line(fd1, NULL);
	free(line);
	close(fd1);
	return (0);
}
