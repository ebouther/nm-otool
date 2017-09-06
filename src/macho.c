/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:37 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/06 15:34:31 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	free_lists(t_sym **sym_lst, t_sect **sect_lst)
{
	t_sym		*lst;
	t_sect		*s_lst;
	void		*tmp;

	lst = *sym_lst;
	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
	s_lst = *sect_lst;
	while (s_lst)
	{
		tmp = s_lst;
		s_lst = s_lst->next;
		free(tmp);
	}
	free(sym_lst);
	free(sect_lst);
}

void	disp_sym_lst(t_sym *lst, t_sect *sect_lst)
{
	t_sect	*begin;
	int		i;

	begin = sect_lst;
	while (lst)
	{
		sect_lst = begin;
		if (lst->n_sect && sect_lst
			&& (lst->type == 'T' || lst->type == 't')
			&& lst->n_sect != NO_SECT)
		{
			i = 1;
			while (sect_lst)
			{
				if (i >= lst->n_sect)
					break ;
				sect_lst = sect_lst->next;
				i++;
			}
			if (i == lst->n_sect)
			{
				lst->type = sect_lst->section +
					((lst->type == 't') ? 32 : 0);
			}
		}
		if (lst->type != 'U')
			ft_printf(lst->arch_64 ? "%016llx" : "%08llx", lst->value);
		else
			ft_printf(lst->arch_64 ? "                " : "        ");
		ft_printf(" %c", lst->type);
		ft_printf(" %s\n", lst->name);
		lst = lst->next;
	}
}

void	sort_sym_lst(t_sym **lst)
{
	uint8_t	sorted;
	t_sym	*sym;
	t_sym	tmp;
	int		diff;

	sorted = 0;
	if (!lst)
		return ;
	while (!sorted)
	{
		sorted = 1;
		sym = *lst;
		while (sym && sym->next)
		{
			if ((diff = ft_strcmp(sym->name, sym->next->name)) > 0
				|| (diff == 0 && sym->value > sym->next->value))
			{
				tmp = *sym;
				sym->value = sym->next->value;
				sym->type = sym->next->type;
				sym->n_sect = sym->next->n_sect;
				sym->name = sym->next->name;
				sym->next->value = tmp.value;
				sym->next->type = tmp.type;
				sym->next->n_sect = tmp.n_sect;
				sym->next->name = tmp.name;
				sorted = 0;
			}
			sym = sym->next;
		}
	}
}

void	add_sect_lst(void *seg, t_sect **sect_lst,
					uint8_t arch_64, uint8_t b_endian)
{
	uint32_t				nsect;
	char					*sectname;
	void					*section;
	uint8_t					n;
	t_sect					*sect;
	t_sect					*last_node;

	nsect = (arch_64 ? ((struct segment_command_64 *)seg)->nsects
			: ((struct segment_command *)seg)->nsects);
	nsect = b_endian ? swap_uint32(nsect) : nsect;
	section = ((void *)seg +
			(arch_64 ? sizeof(struct segment_command_64)
			 : sizeof(struct segment_command)));
	n = 0;
	sect = *sect_lst;
	last_node = NULL;
	while (sect)
	{
		last_node = sect;
		sect = sect->next;
	}
	sect = last_node;
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
		sectname = (arch_64 ? ((struct section_64 *)section)[n].sectname
				: ((struct section *)section)[n].sectname);
		if (ft_strcmp(sectname, SECT_TEXT) == 0)
			sect->section = 'T';
		else if (ft_strcmp(sectname, SECT_DATA) == 0)
			sect->section = 'D';
		else if (ft_strcmp(sectname, SECT_BSS) == 0)
			sect->section = 'B';
		n++;
	}
}

void	add_symtab_lst(int nsyms, int symoff, int stroff, char *ptr,
						t_sym **sym_lst, uint8_t arch_64, uint8_t b_endian)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;
	t_sym			*sym;
	t_sym			*last_node;

	i = 0;
	stringtable = (void *)ptr + stroff;
	sym = *sym_lst;
	last_node = NULL;
	while (sym)
	{
		last_node = sym;
		sym = sym->next;
	}
	sym = last_node;
	while (i < nsyms)
	{
		el = (void *)ptr + symoff +
			(i * (arch_64 ? sizeof(struct nlist_64) : sizeof(struct nlist)));
		el->n_un.n_strx =
			b_endian ? swap_uint32(el->n_un.n_strx) : el->n_un.n_strx;
		el->n_desc = b_endian ? swap_uint16(el->n_desc) : el->n_desc;
		if (ft_strlen(stringtable + el->n_un.n_strx) > 0
			&& !(el->n_type & N_STAB))
		{
			if (sym == NULL)
			{
				sym = (t_sym *)malloc(sizeof(t_sym));
				*sym_lst = sym;
			}
			else
			{
				sym->next = (t_sym *)malloc(sizeof(t_sym));
				sym = sym->next;
			}
			*sym = (t_sym){arch_64, arch_64 ?
				(b_endian ? swap_uint64(el->n_value) : el->n_value)
				: (b_endian ? (uint64_t)swap_uint32(((struct nlist *)el)->n_value) : ((struct nlist *)el)->n_value), 
				'?', el->n_sect, stringtable + el->n_un.n_strx, NULL};
			if ((el->n_type & N_TYPE) == N_UNDF)
				sym->type = (el->n_sect == NO_SECT) ? 'U' : 'C';
			else if ((el->n_type & N_TYPE) == N_ABS)
				sym->type = 'A';
			else if ((el->n_type & N_TYPE) == N_SECT)
				sym->type = 'T';
			else if ((el->n_type & N_TYPE) == N_PBUD)
				sym->type = 'U';
			else if ((el->n_type & N_TYPE) == N_INDR)
				sym->type = 'I';
			else
				sym->type = '?';
			if (!(el->n_type & N_EXT)
					&& sym->type >= 'A'
					&& sym->type <= 'Z')
				sym->type += 32;
		}
		i++;
	}
}

void	nm_macho(char *f, char *ptr, uint8_t mask)
{
	int							ncmds;
	int							i;
	struct mach_header 			*header;
	struct load_command			*lc;
	struct symtab_command		*sym;
	t_sym						**sym_lst;
	t_sect						**sect_lst;

	sym_lst = malloc(sizeof(t_sym *));
	sect_lst = malloc(sizeof(t_sect *));
	*sym_lst = NULL;
	*sect_lst = NULL;

	header = (struct mach_header *)ptr;
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = ((void *)ptr +
		(IS_ARCH_64(mask) ? sizeof(struct mach_header_64)
		 : sizeof(struct mach_header))), IS_BE(mask));
	i = 0;
	if (f)
		ft_printf("\n%s:\n", f);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
			add_sect_lst(lc, sect_lst, IS_ARCH_64(mask), IS_BE(mask));
		else if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			add_symtab_lst(IS_BE(mask) ? swap_uint32(sym->nsyms) : sym->nsyms,
				IS_BE(mask) ? swap_uint32(sym->symoff) : sym->symoff,
				IS_BE(mask) ? swap_uint32(sym->stroff) : sym->stroff,
				ptr, sym_lst, IS_ARCH_64(mask), IS_BE(mask));
		}
		swap_load_command(lc = (void *)lc + lc->cmdsize, IS_BE(mask));
		i++;
	}
	sort_sym_lst(sym_lst);
	disp_sym_lst(*sym_lst, *sect_lst);
	free_lists(sym_lst, sect_lst);
}

void disp_txt_section(void *ptr, void *section, uint8_t mask)
{
	uint64_t	offset;
	uint64_t	size;
	uint64_t	addr;

	offset = (IS_ARCH_64(mask) ?
		(IS_BE(mask) ?
			 swap_uint64(((struct section_64 *)section)->offset)
			: ((struct section_64 *)section)->offset)
		: (IS_BE(mask) ?
			swap_uint32(((struct section *)section)->offset)
			: ((struct section *)section)->offset));
	size = (IS_ARCH_64(mask) ?
		(IS_BE(mask) ?
			swap_uint64(((struct section_64 *)section)->size)
			: ((struct section_64 *)section)->size)
		: (IS_BE(mask) ?
			swap_uint32(((struct section *)section)->size)
			: ((struct section *)section)->size));
	addr = (IS_ARCH_64(mask) ?
			(IS_BE(mask) ? swap_uint64(((struct section_64 *)section)->addr)
			: ((struct section_64 *)section)->addr)
			: (IS_BE(mask) ? swap_uint32(((struct section *)section)->addr)
			: ((struct section *)section)->addr));
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
			disp_txt_section(ptr, &(section[n]), mask);
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
	if (IS_DISP(mask))
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
