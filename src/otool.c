/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:33 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/08 12:00:30 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

int	main(int argc, char **argv)
{
	int		fd;
	int		i;
	uint8_t	ret;

	i = 0;
	while (++i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) < 0)
		{
			disp_err(argv[0], argv[i], "Permission denied.\n");
			return (EXIT_FAILURE);
		}
		ret = nm_otool(fd, argv[i], DISP, OTOOL);
		if (close(fd) == -1)
			return (EXIT_FAILURE);
	}
	return (ret);
}
