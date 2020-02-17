/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 23:35:07 by charmstr          #+#    #+#             */
/*   Updated: 2020/02/17 23:55:58 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 32
#include "./get_next_line.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

int	main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	char *line;
	int fd;
	int	res;

	line = NULL;
	if ((fd = open("/dev/fd/0", O_RDONLY)) == -1)
	{
		printf("failed to openn the fd\n");
		return (0);
	}
	while ((res = get_next_line(fd, &line)) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	if (res == 0)
		free(line);
	return (0);
}
