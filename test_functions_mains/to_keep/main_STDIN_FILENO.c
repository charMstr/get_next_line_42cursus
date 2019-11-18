/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_STDIN_FILENO.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 19:50:33 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/17 22:05:53 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int	main(int argc, char *argv[])
{
	int		fd_user_output;
	int		fd_test_output;
	char	*line;
	int		result;
	char 	buf[BUFFER_SIZE + 1];

	line = NULL;
	result = 1;
	if (argc != 4)
	{
		printf("failed to provide an arg for file to be read\n");
	}
	if((fd_user_output = open(argv[2], O_RDWR | O_TRUNC, 0644)) == -1)
	{
		ft_putstr_fd(2, "failed to open user_output filedescriptor", ft_strlen(
		return (0);
	}
	printf("siz of string : %d\n", ft_strlen("failed to open user_output filedescriptor"));
	return (0);

	printf("press ^D to kill your GNL\n");
	while ((result = get_next_line(atoi(argv[1]), &line)) > 0)
	{
		write(, buff,
		printf("%s\n", line);
	}
	printf("%s\n", line);
	printf("now compare with read behavior:\n");
	while ((result = read(atoi(argv[1]), buf, BUFFER_SIZE)) > 0)
	{
		buf[result] = '\0';
		printf("%s\n", buf);
	}
	free(line);
	return (0);
}

