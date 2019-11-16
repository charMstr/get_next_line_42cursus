/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 13:31:16 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/16 18:53:59 by charmstr         ###   ########.fr       */
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
		printf("in link: %d, fd is:%d\n", link, list->fd);
		if (list->rest)
			printf("link->rest = \"%s\"\n", list->rest);
		else
			printf("link->rest is a NULL pointer\n");
		printf("len_rest is: %d\n", list->len_rest);
		printf("len_line is: %d\n", list->len_line);
		printf("eof is :%d\n", list->eof);
	//while (list)
	//{
		//link++;
		//list = list->next;
	//}
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
			return (-1);
	if (!head)
		if (!(head = new_link(fd)))
			return (-1);
	if(!(fd_link = manage_link(fd, &head, ADD)))
			return (-1);
	fd_link->len_line = 0;
	//fd_links_print(fd_link);
	result = to_read_or_not_to_read(fd_link, line); //result always egale a 1-->problem
	//fd_links_print(fd_link);
	if (result <= 0)
		manage_link(fd, &head, REMOVE);
	return (result);
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

int	to_read_or_not_to_read(fd_gnl *fd_link, char **line)//le problem vient de cette fonction qui renvoie toujours 1!
{
	int		res;
	int		bytes_read;
	char	buf[BUFFER_SIZE + 1];
	int		keep_reading;

	keep_reading = 1;
	if (fd_link->len_rest)
		keep_reading = update_strings(line, fd_link->rest, fd_link->len_rest, fd_link);
	while (keep_reading > 0)
	{
		if ((bytes_read = read(fd_link->fd, buf, BUFFER_SIZE)) == -1)
				return (-1);
		buf[bytes_read] = '\0';
		if (bytes_read < BUFFER_SIZE)
			fd_link->eof = 1;
		keep_reading = update_strings(line, buf, bytes_read, fd_link);
	}
	if (keep_reading < 0)
		return (-1);
	if (fd_link->eof && (fd_link->len_rest == 0))
		return (0);
	return (1);
}

/*
** man: concatenate anything before a '\n' in string with existing line.
**		place the rest in fd_link->rest;
**
** RETURN:	1 if no '\n' was found and fd_link->eof == 0
**			1 if one was found at the very end of buff and fd_link->eof == 0
**
**			0 if a '\n' was found before the end of rest
**			0 if a '\n' is found at the end of rest and fd_link->eof == 1
**
**			-1 if problem occured.
*/

int		update_strings(char **line, char *parse_me, int parse_me_len, fd_gnl *link)
{
	int start_rest;
	int found;

	found = 0;
	if ((start_rest = update_line(line, parse_me, link)) == -1)
		return (-1);
	if (parse_me[start_rest] == '\n')
	{
		start_rest++;
		found = 1;
	}
 	if (!(update_rest(parse_me, link, start_rest)))
		return (-1);
	if ((!found && !link->eof) || (found && !parse_me[start_rest] && !link->eof))
		return (1);
	return (0);
}

/*
** note: will update the string at link->rest with whatever was left after the
** position where the backslash_n was found. free old link->rest.
**
** RETURN: 0 if malloc failed, or 1
*/

int	update_rest(char *str, fd_gnl *link, int start)
{
	int		i;
	char	*tmp;
	int		new_rest;

	tmp = link->rest;
	i = 0;
	new_rest = 0;
	while (str[start + i])
		i++;
	if (!(link->rest = (char*)malloc(sizeof(char) * (i + 1))))
		return (0);
	link->rest[i] = '\0';
	while (new_rest < i)
	{
		link->rest[new_rest] = str[start + new_rest];
		new_rest++;
	}
	(tmp != NULL) ? free(tmp) : 0;
	link->len_rest = new_rest;
	return (1);
}

/*
** note: function will join the line and the given string until the end of that
** string or until a '\n' is met.
**
** note: fd_link->len_line is updated.
**
** RETURN: the position '\n' was met or the size of the string itself, or -1 if
** a problem occured
*/

int	update_line(char **line, char *str2, fd_gnl *link)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = *line;
	i = 0;
	j = -1;
	while (str2[i] && (str2[i] != '\n'))
		i++;
	if (!(*line = (char*)malloc(sizeof(char) * (link->len_line + i + 1))))
		return (-1);
	(*line)[link->len_line + i] = '\0';
	while (++j < link->len_line + i)
	{
		if (j < link->len_line)
			(*line)[j] = tmp[j];
		else
			(*line)[j] = str2[j - link->len_line];
	}
	link->len_line = link->len_line + i;
	if (tmp)
		free(tmp);
	return (i);
}

int	main(void)
{
	int		fd1;
	char	*line;
	int	bytes_read;
	int		result;

	line = NULL;
	if((fd1 = open("./file_lines1", O_RDONLY)) == -1)
		return (0);
	while ((result = get_next_line(fd1, &line)) > 0)
	{
		printf("%s\n", line);
		for (int a = 0 ; a < 100000000 ; a++)
			;
	}
		printf("%s\n", line);
	get_next_line(fd1, NULL);
	if (line)
		free(line);
	close(fd1);
	return (0);
}
