/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 22:29:55 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/22 22:29:58 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "GNL_TESTS.h"		///to be removed

void	display_struct(t_fd *link, char *line, int result);

int	get_next_line(int fd, char **line)
{
	static t_fd		*head;
	t_fd			*fd_link;
	int				result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (-1);
	if (!line)
		if (!manage_link(fd, &head, REMOVE))
			return (-1);
	*line = NULL;
	if (!head)
		if (!(head = new_link(fd)))
			return (-1);
	if (!(fd_link = manage_link(fd, &head, ADD)))
		return (-1);
	fd_link->len_line = 0;
	fd_link->b_zero = 0;
	*line = 0;
	result = to_read_or_not_to_read(fd_link, line);
	if (result <= 0)
		manage_link(fd, &head, REMOVE);
//	display_struct(fd_link, *line, result);
//	for (int i = 0; i < 100000000; i++) ;
	return (result);
}

void	display_struct(t_fd *link, char *line, int result)
{
	my_ft_putstr_fd("\n-------------------------\n", 1);
	my_ft_putstr_fd("line is: |", 1);
	my_ft_putstr_fd(line, 1);
	my_ft_putstr_fd("|\nlen_line is: ", 1);
	my_ft_putnbr_fd(link->len_line, 1);
	my_ft_putstr_fd("\nrest is: |", 1);
	my_ft_putstr_fd(link->rest, 1);
	my_ft_putstr_fd("|\nlen_rest is: ", 1);
	my_ft_putnbr_fd(link->len_rest, 1);
	my_ft_putstr_fd("\nEOF is: ", 1);
	my_ft_putnbr_fd(link->eof, 1);
	my_ft_putstr_fd("\nb_zero is: ", 1);
	my_ft_putnbr_fd(link->b_zero, 1);
	my_ft_putstr_fd("\ncoming out of GNL call: result is:", 1);
	my_ft_putnbr_fd(result, 1);
	my_ft_putstr_fd("\n-------------------------\n\n\n", 1);
}

int	to_read_or_not_to_read(t_fd *link, char **line)
{
	int		bytes_read;
	char	buf[BUFFER_SIZE + 1];
	int		position;

	position = BUFFER_SIZE;
	if (link->len_rest)
		if ((position = enter_next_loop(line, link)) == -1)
			return (-1);
	while (position == BUFFER_SIZE && !link->eof && !link->b_zero)
	{
		if ((bytes_read = read(link->fd, buf, BUFFER_SIZE)) == -1)
			return (-1);
		if (bytes_read < BUFFER_SIZE)
			link->eof = 1;
		if ((position = update_line(line, link, buf, bytes_read)) == -1)
			return (-1);
		if (!update_rest(buf, position, bytes_read, link))
			return (-1);
	}
	if (link->eof && !link->len_rest)
		return (0);
	return (link->b_zero == 1) ? 2 : 1;
}

/*
** note:	this function will be called if there was some left-overs in
**			fd->rest, from the previous call to get_next_line on that fd.
**
**			if yes it will update both line and rest starting at + 1 index..
**
** RETURN:	BUFFER_SIZE to validate the next loop condition if length of rest
**					is now 0
**			0 if there is still characters in rest string.
**			-1 if a problem occured.
*/

int	enter_next_loop(char **line, t_fd *link)
{
	int	position;

	if ((position = update_line(line, link, link->rest + 1 , \
					link->len_rest - 1)) == -1)
		return (-1);
	if (!update_rest(link->rest + 1, position , link->len_rest - 1 , link))
		return (-1);
	if (link->len_rest == 0)
		return (BUFFER_SIZE);
	return (0);
}

/*
** note:	this function will update line, and if a binary_zero value is found
**			before the lenght of bytes toi be read, then the fd->b_zero flag is
**			activated.
**
** RETURN:	the position where the END_LINE_CHAR or '\0' was found.
*/

int	update_line(char **line, t_fd *link, const char *src, int len_src)
{
	int		i;
	int		j;
	char	*tmp;

	tmp = *line;
	i = -1;
	j = -1;
	while (++i < len_src)
		if (!src[i] || src[i] == END_LINE_CHAR)
			break;
	link->b_zero = (src[i] == '\0' && i < len_src) ? 1 : 0;
	if (!(*line = (char*)malloc(sizeof(char) * (link->len_line + i + 1))))
		return (-1);
	(*line)[link->len_line + i] = '\0';
	while (++j < link->len_line + i)
	{
		if (j < link->len_line)
			(*line)[j] = tmp[j];
		else
			(*line)[j] = src[j - link->len_line];
	}
	link->len_line = link->len_line + i;
	free(tmp);
	return (i);
}

/*
** note:	this function will update the string fd->rest. and te rest_len,
**			reading in the char *str, starting at start position, until len_str
**			char are read.
**
** RETURN:	1 if ok
**			0 if malloc failed
*/

int	update_rest(const char *str, int start, int len_str, t_fd *link)
{
	int		i;
	char	*tmp;
	int		len;

	len = (len_str - start < 0) ? 0 : len_str - start;
	tmp = link->rest;
	i = -1;
	if (!(link->rest = (char*)malloc(sizeof(char) * (len + 1))))
		return (0);
	link->rest[len] = '\0';
	while (++i < len)
			link->rest[i] = str[start + i];
	link->len_rest = len;
	free(tmp);
	return (1);
}
