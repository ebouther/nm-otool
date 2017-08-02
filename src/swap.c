/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:32:57 by ebouther          #+#    #+#             */
/*   Updated: 2017/06/07 19:01:26 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "swap.h"

uint32_t	swap_uint32(uint32_t x)
{
	return (((x & 0xff000000) >> 24) | 
			((x & 0x00ff0000) >>  8) | 
			((x & 0x0000ff00) <<  8) | 
			((x & 0x000000ff) << 24));
}

void swap_fat_header(struct fat_header *fat_header, uint8_t l_endian)
{
	if (!l_endian)
		return ;
	fat_header->magic = swap_uint32(fat_header->magic);
	fat_header->nfat_arch = swap_uint32(fat_header->nfat_arch);
}

void swap_fat_arch(struct fat_arch *fat_archs, uint32_t nfat_arch,uint8_t l_endian)
{
	uint32_t i;

	if (!l_endian)
		return ;
	i = 0;
	while (i < nfat_arch)
	{
		fat_archs[i].cputype    = swap_uint32(fat_archs[i].cputype);
		fat_archs[i].cpusubtype = swap_uint32(fat_archs[i].cpusubtype);
		fat_archs[i].offset     = swap_uint32(fat_archs[i].offset);
		fat_archs[i].size       = swap_uint32(fat_archs[i].size);
		fat_archs[i].align      = swap_uint32(fat_archs[i].align);
		i++;
	}
}

