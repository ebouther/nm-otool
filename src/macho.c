/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:37 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/11 18:54:45 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	parse_lc(t_sym **sym_lst, t_sect **sect_lst, char *ptr, uint8_t mask)
{
	int						i;
	int						ncmds;
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header		*header;

	header = (struct mach_header *)ptr;
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = ((void *)ptr +
		(IS_ARCH_64(mask) ? sizeof(struct mach_header_64)
		: sizeof(struct mach_header))), IS_BE(mask));
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
			add_sect_lst(lc, sect_lst, mask);
		else if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			add_symtab_lst(sym, ptr, sym_lst, mask);
		}
		swap_load_command(lc = (void *)lc + lc->cmdsize, IS_BE(mask));
		i++;
	}
}

void	nm_macho(char *f, char *ptr, uint8_t mask)
{
	t_sym						**sym_lst;
	t_sect						**sect_lst;

	sym_lst = malloc(sizeof(t_sym *));
	sect_lst = malloc(sizeof(t_sect *));
	*sym_lst = NULL;
	*sect_lst = NULL;
	if (f)
		ft_printf("\n%s:\n", f);
	parse_lc(sym_lst, sect_lst, ptr, mask);
	sort_sym_lst(sym_lst);
	disp_sym_lst(*sym_lst, *sect_lst);
	free_lists(sym_lst, sect_lst);
}

void	disp_txt_section(void *ptr, void *section, uint8_t mask)
{
	uint64_t	offset;
	uint64_t	size;
	uint64_t	addr;

	if (IS_ARCH_64(mask))
	{
		offset = (IS_BE(mask) ?
				swap_uint64(((struct section_64 *)section)->offset)
				: ((struct section_64 *)section)->offset);
		size = IS_BE(mask) ? swap_uint64(((struct section_64 *)section)->size)
				: ((struct section_64 *)section)->size;
		addr = IS_BE(mask) ? swap_uint64(((struct section_64 *)section)->addr)
				: ((struct section_64 *)section)->addr;
	}
	else
	{
		offset = (IS_BE(mask) ? swap_uint32(((struct section *)section)->offset)
			: ((struct section *)section)->offset);
		size = IS_BE(mask) ? swap_uint32(((struct section *)section)->size)
			: ((struct section *)section)->size;
		addr = IS_BE(mask) ? swap_uint32(((struct section *)section)->addr)
			: ((struct section *)section)->addr;
	}
	hexdump(ptr + offset, size, addr, mask);
}

void	otool_section(void *seg, char *ptr, uint8_t mask)
{
	uint32_t				nsect;
	char					*sectname;
	void					*section;
	uint8_t					n;

	nsect = (IS_ARCH_64(mask) ? ((struct segment_command_64 *)seg)->nsects
			: ((struct segment_command *)seg)->nsects);
	nsect = IS_BE(mask) ? swap_uint32(nsect) : nsect;
	section = ((void *)seg +
			(IS_ARCH_64(mask) ? sizeof(struct segment_command_64)
			: sizeof(struct segment_command)));
	n = 0;
	while (n < nsect)
	{
		sectname =
			(IS_ARCH_64(mask) ? ((struct section_64 *)section)->sectname
			: ((struct section *)section)->sectname);
		if (ft_strcmp(sectname, SECT_TEXT) == 0)
		{
			disp_txt_section(ptr, &(section[n]), mask);
			break ;
		}
		n++;
	}
}

void	otool_macho(char *f, char *ptr, uint8_t mask)
{
	int							ncmds;
	int							i;
	struct mach_header			*header;
	struct load_command			*lc;

	header = (struct mach_header *)ptr;
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = (void *)ptr + (IS_ARCH_64(mask) ?
		sizeof(struct mach_header_64)
		: sizeof(struct mach_header)), IS_BE(mask));
	i = 0;
	if (f && IS_DISP(mask))
		ft_printf("%s:\n", f);
	ft_printf("Contents of (__TEXT,__text) section\n");
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
			otool_section(lc, ptr, mask);
		swap_load_command(lc = (void *)lc + lc->cmdsize, IS_BE(mask));
		i++;
	}
}

void	handle_macho(char *f, char *ptr, uint8_t mask)
{
	if (IS_NM(mask))
		nm_macho(IS_HIDDEN(mask) ? NULL : f, ptr, mask);
	else
		otool_macho(f, ptr, mask);
}
