/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 16:14:41 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/18 21:28:14 by charmstr         ###   ########.fr       */
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

	if (fd < 0 || BUFFER_SIZE == 0)
		return (-1);
	if (!line)
		if (!manage_link(fd, &head, REMOVE))
			return (-1);
	if (!head)
		if (!(head = new_link(fd)))
			return (-1);
	if (!(fd_link = manage_link(fd, &head, ADD)))
		return (-1);
	fd_link->len_line = 0;
	result = to_read_or_not_to_read(fd_link, line);
	if (result <= 0)
		manage_link(fd, &head, REMOVE);
	return (result);
}

/*
** man:		should read until (an END_LINE_CHAR is reached and is not at the
**			end of the buffer) OR (EOF is reachedi). line is updated until the
**			first END_LINE_CHARR is met.
**
** note:	whatever followed the first END_LINE_CHAR is stored into fd->rest.
**
** RETURN:	-1	if a problem occured
**			0	if EOF reached and there is nothing left in link->rest string.
**			1	if there is more lines to be printed out.
*/

int	to_read_or_not_to_read(t_fd *fd_link, char **line)
{
	int		bytes_read;
	char	buf[BUFFER_SIZE + 1];
	int		keep_going;

	keep_going = 1;
	if (fd_link->len_rest)
		keep_going = update_strings(line, fd_link->rest, fd_link, keep_going);
	while (keep_going > 0 && !fd_link->eof)
	{
		bytes_read = read(fd_link->fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (-1);
		buf[bytes_read] = '\0';
		if (bytes_read > 0 && buf[bytes_read - 1] == END_LINE_CHAR)
			keep_going = update_strings(line, buf, fd_link, keep_going);
		else
			keep_going = update_strings(line, buf, fd_link, keep_going);
		if (bytes_read < BUFFER_SIZE)
			fd_link->eof = 1;
	}
	if (keep_going < 0)
		return (-1);
	return ((fd_link->eof && !fd_link->len_rest) ? 0 : 1);
}

/*
** man: 	concatenate anything found before an END_LINE_CHAR in the string
**			with existing the line. place the rest in fd_link->rest.
**
** note:	INPUTS for tricky: 1 or 2.
**			if 2: only rest is updatedit will return 2 if we keep finding an
**			END_LINE_CHAR in the tricky spot until we can RETURN 0.
**
** RETURN:	-1 if a problem occured.
**			0 if a END_LINE_CHAR was found. stop reading
**			1 if no END_LINE_CHAR occured
**			2 if a END_LINE_CHAR was but in the tricky spot(last char in buf)
*/

int	update_strings(char **line, char *parse_me, t_fd *link, int tricky)
{
	int start_rest;
	int len;
	int found;

	found = 0;
	start_rest = 0;
	if (tricky == 1)
		if ((start_rest = update_line(line, parse_me, link, &found)) == -1)
			return (-1);
	len = update_rest(parse_me, link, start_rest);
	if (len < 0)
		return (-1);
	if (len == BUFFER_SIZE && len && parse_me[len - 1] == END_LINE_CHAR)
		return (2);
	if (tricky == 2)
		return (0);
	if (!found)
		return (1);
	return (0);
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

int	update_line(char **line, char *str2, t_fd *link, int *found)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = *line;
	i = 0;
	j = -1;
	while (str2[i] && (str2[i] != END_LINE_CHAR))
		i++;
	if (str2[i] == END_LINE_CHAR)
		*found = 1;
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
	free(tmp);
	return (i);
}

/*
** note: 	will update the string link->rest with whatever was left after the
**			position where the END_LINE_CHAR was found. free old link->rest.
**
** RETURN:	-1 if malloc failed
**			or the length of the string str.
*/

int	update_rest(char *str, t_fd *link, int start)
{
	int		i;
	char	*tmp;
	int		new_rest;

	tmp = link->rest;
	i = 0;
	new_rest = 0;
	if (str[start] == END_LINE_CHAR)
		start++;
	while (str[start + i])
		i++;
	if (!(link->rest = (char*)malloc(sizeof(char) * (i + 1))))
		return (-1);
	link->rest[i] = '\0';
	while (new_rest < i)
	{
		link->rest[new_rest] = str[start + new_rest];
		new_rest++;
	}
	free(tmp);
	link->len_rest = new_rest;
	return (i);
}
