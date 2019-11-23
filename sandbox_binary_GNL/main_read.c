/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_read.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charmstr <charmstr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 23:26:28 by charmstr          #+#    #+#             */
/*   Updated: 2019/11/23 22:07:58 by charmstr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "GNL_TESTS.h"

int	main(int argc, char *argv[])
{
	int 	fd_test_me;
	//int		fd_read_output;
	char	*line;
	int		result;
	char 	buf[BUFFER_SIZE + 1];

	line = NULL;
	result = 1;
	if (argc != 2)
	{
		my_ft_putstr_fd("failed to provide the file to be opened as arg\n", 2);
		return (0);
	}
	/*
	if((fd_read_output = open("./read_output", O_RDWR | O_CREAT |  O_TRUNC, 0644)) == -1)
	{
		my_ft_putstr_fd("failed to open ./read_output filedescriptor\n", 2);
		return (0);
	}
	*/
	if((fd_test_me = open(argv[1], O_RDONLY)) == -1)
	{
		my_ft_putstr_fd("failed to open", 2);
		my_ft_putstr_fd(argv[1], 2);
		my_ft_putstr_fd("filedescriptor.\n", 2);
		return (0);
	}
	while (result)
	{
		result = read(fd_test_me, buf, BUFFER_SIZE);
		if (result != -1)
		{
			if (result)
				my_ft_putstr_fd(buf, 1);
			if (result == 1)
				my_ft_putchar_fd('\n', 1);
		}
	}
	close(fd_test_me);
	//close(fd_read_output);
	return (0);
}
