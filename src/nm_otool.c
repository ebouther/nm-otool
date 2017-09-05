/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:24 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/05 19:55:57 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void		handlers(char *f, char *ptr, uint8_t mask)
{
	unsigned int	magic_number;

	magic_number = *(unsigned int *)ptr;
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		handle_macho(f, ptr,
			SET_ARCH_32(((magic_number == MH_MAGIC) ? SET_LE(mask) : SET_BE(mask))));
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_macho(f, ptr,
			SET_ARCH_64(((magic_number == MH_MAGIC_64) ? SET_LE(mask) : SET_BE(mask))));
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(f, ptr, (magic_number == FAT_MAGIC) ? SET_LE(mask) : SET_BE(mask));
	else if (magic_number == *((unsigned int *)ARMAG))
		handle_ar(f, ptr);
	else
		disp_err(f, " The file was not recognized as a valid object file.\n");
}

uint8_t		nm_otool(int fd, char *file, uint8_t disp, uint8_t mask)
{
	char		*ptr;
	struct stat	buf;

	if (fstat(fd, &buf) < 0)
		return (-1);
	if ((ptr = mmap(0, buf.st_size,
			PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (-1);
	handlers(file, ptr, disp ? SET_DISP(mask) : mask);
	if (munmap(ptr, buf.st_size) < 0)
		return (-1);
	return (0);
}
