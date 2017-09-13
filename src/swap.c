/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:32:57 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/13 18:42:46 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "swap.h"

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

int			swap_nlist(struct nlist_64 *el, uint8_t b_endian)
{
	if (((unsigned long)el + sizeof(*el)) > g_offset)
		return (-1);
	el->n_un.n_strx =
		b_endian ? swap_uint32(el->n_un.n_strx) : el->n_un.n_strx;
	el->n_desc = b_endian ? swap_uint16(el->n_desc) : el->n_desc;
	return (0);
}
