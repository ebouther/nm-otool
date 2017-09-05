/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:33 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/05 19:54:39 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

int	main(int argc, char **argv)
{
	int	fd;
	int	i;

	i = 0;
	while (++i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) < 0)
		{
			disp_err(argv[i], "Permission denied.\n");
			continue ;
		}
		nm_otool(fd, argv[i], DISP, OTOOL);
		if (close(fd) == -1)
			continue;
	}
	return (0);
}
