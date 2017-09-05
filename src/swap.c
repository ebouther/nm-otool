/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:32:57 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/05 20:00:37 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "swap.h"

uint32_t	swap_uint32(uint32_t x)
{
	return (((x & 0xff000000) >> 24) |
			((x & 0x00ff0000) >> 8) |
			((x & 0x0000ff00) << 8) |
			((x & 0x000000ff) << 24));
}

uint16_t	swap_uint16(uint16_t x)
{
	return (((x & 0xff00) >> 8) |
			((x & 0x00ff) << 8));
}

uint64_t	swap_uint64(uint64_t x)
{
	return (((x & 0xff00000000000000ULL) >> 56) |
			((x & 0x00ff000000000000ULL) >> 40) |
			((x & 0x0000ff0000000000ULL) >> 24) |
			((x & 0x000000ff00000000ULL) >> 8) |
			((x & 0x00000000ff000000ULL) << 8) |
			((x & 0x0000000000ff0000ULL) << 24) |
			((x & 0x000000000000ff00ULL) << 40) |
			((x & 0x00000000000000ffULL) << 56));
}

void		swap_fat_header(struct fat_header *fat_header, uint8_t b_endian)
{
	if (!b_endian)
		return ;
	fat_header->magic = swap_uint32(fat_header->magic);
	fat_header->nfat_arch = swap_uint32(fat_header->nfat_arch);
}

void		swap_load_command(struct load_command *lc, uint8_t b_endian)
{
	if (!b_endian)
		return ;
	lc->cmd = swap_uint32(lc->cmd);
	lc->cmdsize = swap_uint32(lc->cmdsize);
}

void		swap_fat_arch(struct fat_arch *fat_archs,
							uint32_t nfat_arch,
							uint8_t b_endian)
{
	uint32_t i;

	if (!b_endian)
		return ;
	i = 0;
	while (i < nfat_arch)
	{
		fat_archs[i].cputype = swap_uint32(fat_archs[i].cputype);
		fat_archs[i].cpusubtype = swap_uint32(fat_archs[i].cpusubtype);
		fat_archs[i].offset = swap_uint32(fat_archs[i].offset);
		fat_archs[i].size = swap_uint32(fat_archs[i].size);
		fat_archs[i].align = swap_uint32(fat_archs[i].align);
		i++;
	}
}
