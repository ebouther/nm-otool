/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 20:02:06 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/08 11:59:55 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	disp_err(char *bin, char *f, char *err)
{
	ft_putstr_fd(bin, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(f, 2);
	ft_putstr_fd(err, 2);
}

void	hexdump(void *data, size_t size, uint64_t addr, uint8_t mask)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < size)
	{
		IS_ARCH_64(mask) ? ft_printf("%016llx\t", addr + i)
			: ft_printf("%08llx\t", addr + i);
		j = 0;
		while (j < 16
				&& i + j < size)
		{
			ft_printf("%02x", ((uint8_t *)data)[i + j++]);
			if (IS_DUMP_1(mask) || (IS_DUMP_4(mask) && j % 4 == 0))
				ft_putchar(' ');
		}
		ft_putchar('\n');
		if ((i += 16) >= size)
			break ;
	}
}
