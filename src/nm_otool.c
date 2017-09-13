/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:24 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/13 19:20:43 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

uint8_t		handlers(char *f, char *ptr, uint8_t mask)
{
	unsigned int	magic_number;

	if ((unsigned long)ptr > g_offset)
		return (EXIT_FAILURE);
	magic_number = *(unsigned int *)ptr;
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		return (handle_macho(f, ptr,
			SET_ARCH_32(((magic_number == MH_MAGIC) ?
					SET_LE(mask) : SET_BE(mask)))));
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		return (handle_macho(f, ptr,
			SET_ARCH_64(((magic_number == MH_MAGIC_64) ?
					SET_LE(mask) : SET_BE(mask)))));
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(f, ptr, (magic_number == FAT_MAGIC)
				? SET_LE(mask) : SET_BE(mask));
	else if (magic_number == *((unsigned int *)ARMAG))
		handle_ar(f, ptr, mask);
	else
	{
		disp_err(IS_NM(mask) ? "ft_nm" : "ft_otool", f,
			" The file was not recognized as a valid object file.\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

uint8_t		nm_otool(int fd, char *file, uint8_t mask)
{
	char		*ptr;
	struct stat	buf;
	uint8_t		ret;

	if (fstat(fd, &buf) < 0)
		return (EXIT_FAILURE);
	if ((ptr = mmap(0, buf.st_size,
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (EXIT_FAILURE);
	g_offset = (unsigned long)ptr + buf.st_size - 1;
	ret = handlers(file, ptr, mask);
	if (munmap(ptr, buf.st_size) < 0)
		return (EXIT_FAILURE);
	return (ret);
}
