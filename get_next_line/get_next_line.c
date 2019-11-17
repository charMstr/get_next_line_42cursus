/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:14:41 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/17 23:56:19 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** note1: 	2nd set of conditions -> added feature:
**			if get_next_line() is called with NULL pointer it will remove/free
**			the potentially existing link matching the given filedescriptor.
**
** note2:	3rd set of conditions -> called only on the very frist time (static
**			pointer initialized to NULL).
**
** RETURN: -1 error occured
**			0 EOF is reached, and there is nothing left in "fd->rest" string
**			1 the END_LINE_CHAR was met and there is still things to be printed
**				out.
*/

int	get_next_line(int fd, char **line)
{
	static t_fd		*head;
	t_fd			*fd_link;
	int				result;

	if (fd < 0)
		return (-1);
	if (!line)
		if (!manage_link(fd, &head, REMOVE))
			return (-1);
	if (!head)
		if (!(head = new_link(fd)))
			return (-1);
	if (!(fd_link = manage_link(fd, &head, ADD)))
		return (-1);
	result = to_read_or_not_to_read(fd_link, line);
	if (result <= 0)
		manage_link(fd, &head, REMOVE);
	return (result);
}

/*
** man:		should read until an END_LINE_CHAR or EOF is reached and each time
**			will update line.
**
** note:	whatever followed the first END_LINE_CHAR is stored  into fd->rest
**
** RETURN:	-1	if a problem occured
**			0	if EOF reached and there is nothing left in link->rest string
**			1	if EOF hasen't been reached yet.
*/

int	to_read_or_not_to_read(t_fd *fd_link, char **line)
{
	int		bytes_read;
	char	buf[BUFFER_SIZE + 1];
	int		end_of_line;
	int		len_line;

	len_line = 0;
	end_of_line = 0;
	if (fd_link->len_rest)
		if ((end_of_line = update_strings(line, fd_link->rest, fd_link, \
						&len_line)) < 0)
			return (-1);
	while (!end_of_line && !fd_link->eof)
	{
		if ((bytes_read = read(fd_link->fd, buf, BUFFER_SIZE)) == -1)
			return (-1);
		buf[bytes_read] = '\0';
		if ((end_of_line = update_strings(line, buf, fd_link, &len_line)) < 0)
			return (-1);
		if (!bytes_read || (bytes_read < BUFFER_SIZE && !fd_link->len_rest))
			fd_link->eof = 1;
	}
	if (fd_link->eof && (fd_link->len_rest == 0))
		return (0);
	return (1);
}

/*
** man: 	concatenate anything found before an END_LINE_CHAR in the string
**			with existing the line. place the rest in fd_link->rest.
**
** RETURN:	1 if a END_LINE_CHAR was found
**			0 if no END_LINE_CHAR was found
**			-1 if problem occured.
*/

int	update_strings(char **line, char *parse_me, t_fd *link, int *len_line)
{
	int start_rest;
	int found;

	found = 0;
	if ((start_rest = update_line(line, parse_me, len_line)) == -1)
		return (-1);
	if (parse_me[start_rest] == END_LINE_CHAR)
	{
		start_rest++;
		found = 1;
	}
	if (!(update_rest(parse_me, link, start_rest)))
		return (-1);
	return (found);
}

/*
** note:	this function will join the line and the given string until the end
**			of that string or until a END_LINE_CHAR is met.
**
** note:	fd_link->len_line is updated.
**
** RETURN:	the position END_LINE_CHAR was met
**			or the size of the stringitself,
**			or -1 if a problem occured
*/

int	update_line(char **line, char *str2, int *len_line)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = *line;
	i = 0;
	j = -1;
	while (str2[i] && (str2[i] != END_LINE_CHAR))
		i++;
	if (!(*line = (char*)malloc(sizeof(char) * ((*len_line) + i + 1))))
		return (-1);
	(*line)[(*len_line) + i] = '\0';
	while (++j < (*len_line) + i)
	{
		if (j < *len_line)
			(*line)[j] = tmp[j];
		else
			(*line)[j] = str2[j - (*len_line)];
	}
	*len_line = (*len_line) + i;
	free(tmp);
	return (i);
}

/*
** note: 	will update the string link->rest with whatever was left after the
**			position where the END_LINE_CHAR was found. free old link->rest.
**
** RETURN:	0 if malloc failed
**			1 if job done normally
*/

int	update_rest(char *str, t_fd *link, int start)
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
	free(tmp);
	link->len_rest = new_rest;
	return (1);
}
