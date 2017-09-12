/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 18:25:35 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/12 18:25:49 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <symbols.h>

t_sect	*last_node_sect(t_sect *sect)
{
	t_sect			*last_node;

	last_node = NULL;
	while (sect)
	{
		last_node = sect;
		sect = sect->next;
	}
	return (last_node);
}

void	set_sect(t_sect *sect, void *seg, uint8_t n, uint8_t mask)
{
	void					*section;
	char					*sectname;
	char					*segname;

	section = ((void *)seg +
			(IS_ARCH_64(mask) ? sizeof(struct segment_command_64)
			: sizeof(struct segment_command)));
	sectname = (IS_ARCH_64(mask) ? ((struct section_64 *)section)[n].sectname
			: ((struct section *)section)[n].sectname);
	segname = (IS_ARCH_64(mask) ? ((struct section_64 *)section)[n].segname
			: ((struct section *)section)[n].segname);
	if (ft_strcmp(sectname, SECT_TEXT) == 0)
		sect->section = 'T';
	else if (ft_strcmp(sectname, SECT_DATA) == 0)
		sect->section = 'D';
	else if (ft_strcmp(sectname, SECT_BSS) == 0)
		sect->section = 'B';
}

void	add_sect_lst(void *seg, t_sect **sect_lst, uint8_t mask)
{
	uint32_t				nsect;
	uint8_t					n;
	t_sect					*sect;

	nsect = (IS_ARCH_64(mask) ? ((struct segment_command_64 *)seg)->nsects
			: ((struct segment_command *)seg)->nsects);
	nsect = IS_BE(mask) ? swap_uint32(nsect) : nsect;
	n = 0;
	sect = last_node_sect(*sect_lst);
	while (n < nsect)
	{
		if (sect == NULL)
		{
			sect = (t_sect *)malloc(sizeof(t_sect));
			*sect_lst = sect;
		}
		else
		{
			sect->next = (t_sect *)malloc(sizeof(t_sect));
			sect = sect->next;
		}
		*sect = (t_sect){'S', NULL};
		set_sect(sect, seg, n, mask);
		n++;
	}
}
