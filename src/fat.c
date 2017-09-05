/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 14:29:50 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/04 16:07:56 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	get_all_arch(char *f, char *ptr, uint8_t mask)
{
	struct fat_arch		*arch;
	struct fat_header	*header;
	uint32_t			i;

	i = 0;
	header = (struct fat_header *)ptr;
	arch = (void *)ptr + sizeof(*header);
	while (i < header->nfat_arch)
	{
		if (header->nfat_arch > 1)
			ft_printf(IS_NM(mask) ? "\n%s (for architecture %s):\n"
				: "%s (architecture %s):\n", f,
				NXGetArchInfoFromCpuType(arch->cputype,
										arch->cpusubtype)->name);
		else
			ft_printf("%s:\n", f);
		handlers(f, (void *)ptr + arch->offset,
			SET_HIDE(((arch->cputype == CPU_TYPE_X86_64
			|| arch->cputype == CPU_TYPE_I386) ? DUMP_1(mask) : DUMP_4(mask))));
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
}

void	handle_fat(char *f, char *ptr, uint8_t mask)
{
	uint32_t					i;
	struct fat_header			*header;
	struct fat_arch				*arch;

	swap_fat_header((header = (struct fat_header *)ptr), IS_BE(mask));
	swap_fat_arch(arch = (void *)ptr
			+ sizeof(*header), header->nfat_arch, IS_BE(mask));
	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch->cputype == CPU_TYPE_X86_64)
			return (handlers(f, (void *)ptr + arch->offset, SET_DISP(mask)));
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
	get_all_arch(f, ptr, mask);
}
