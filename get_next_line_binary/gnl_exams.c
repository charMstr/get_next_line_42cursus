/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 21:07:42 by charmstr          #+#    #+#             */
/*   Updated: 2020/02/17 23:48:45 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 32

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}


// this function looks for a backslash n
// returns 1 if found it, else 0
// we update the position of the index in the buffer no matter what.

int	find_bn_or_b0(char *buf, int *i)
{
	while (buf[*i] && buf[*i] != '\n')
	{
		(*i)++;
	}
	if (buf[*i] == '\n')
		return (1);
	return (0);
}

// update the line with whatever was before the i index  in the buffer.
// add a '\0'
// returns 0 if the malloc failed...... fuck that

int update_line(char **line, char *buf, int i)
{
	int j;
	int	size;
	int	size_initial;
	char *tmp;

	tmp = *line;
	size_initial = ft_strlen(tmp);
	size = size_initial + i;
	j = 0;
	if (!(*line = malloc(sizeof(char) * size + 1)))
		return (0);
	line[0][size] = '\0';
	while (j < size)
	{
		if (j < size_initial)
			line[0][j] = tmp[j];
		else
			line[0][j] = buf[j - size_initial];
		j++;
	}
	free(tmp);
	return (1);
}

//this function will update the buffer and basicaly do a memcopy from the end to the beginning.
//if the i character was actually a '\n', then we skip it.
void 	update_buf(char *buf, int i)
{
	int j;

	j = 0;
	if (buf[i] == '\n')
		i++;
	while (buf[i])
	{
		buf[j] = buf[i];
		i++;
		j++;
	}
	buf[j] = '\0';
}

int	get_next_line(int fd, char **line)
{
	static char buf[BUFFER_SIZE + 1] = {'\0'};
	int			res;
	int			i;
	int			found;

	*line = NULL; 								//set *line to null no matter what.
	if (!line || fd < 0 || BUFFER_SIZE <= 0) 	//OK check for shitty inputs.
		return (-1);
	if ((read(fd, &buf, 0)) == -1)				//OK check if the fd is valid or not;
		return (-1);
	i = 0;
	found = find_bn_or_b0(buf, &i);
	update_line(line, buf, i);					//append buf to line while < i.
	update_buf(buf, i);							//copy whatever is AFTER the '\n' at the beginning of buffer.
	if (found == 1)
		return (found);
	while ((res = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		i = 0;
		buf[res] = '\0';
		found = find_bn_or_b0(buf, &i);
		update_line(line, buf, i);
		update_buf(buf, i);
		if (found == 1)
			return (found);
	}
	return (res); 						//we only get here if the res was 0 or -1.
}


int	main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	char *line;
	int fd;
	int	res;

	line = NULL;
	if ((fd = open("./main_test.c", O_RDONLY)) == -1)
	{
		printf("failed to openn the fd\n");
		return (0);
	}
	while ((res = get_next_line(3, &line)) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	if (res == 0)
		free(line);
	return (0);
}
