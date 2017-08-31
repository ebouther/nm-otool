#include "nm_otool.h"

void free_lists(t_sym **sym_lst, t_sect **sect_lst)
{
	t_sym *lst;
	t_sect *s_lst;
	void	*tmp;

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

void disp_sym_lst(t_sym *lst, t_sect *sect_lst)
{
	t_sect	*begin;
	int		i;

	begin = sect_lst;
	while (lst)
	{
		    //ft_printf("SYMBOL 2 [%x] [%s] [%d]\n", (unsigned int)lst, lst->name, lst->n_sect);
			sect_lst = begin;
			if (lst->n_sect && sect_lst
				&& (lst->type == 'T' || lst->type == 't')
				&& lst->n_sect != NO_SECT)
			{
				i = 1;
				while (sect_lst)
				{
					if (i >= lst->n_sect)
						break;
					sect_lst = sect_lst->next;
					i++;
				}
				if (i == lst->n_sect)
				{
					lst->type = sect_lst->section +
						((lst->type == 't') ? 32 : 0);
				}
			}
			//ft_printf("ARCH64 : %d\n", lst->arch_64);
			if (lst->type != 'U')
				ft_printf(lst->arch_64 ? "%016llx" : "%08llx", lst->value);
			else
				ft_printf(lst->arch_64 ? "                " : "        ");
			ft_printf(" %c", lst->type);
			//ft_printf(" %d", lst->n_sect); // DBG
			ft_printf(" %s\n", lst->name);
			//ft_printf(" N_SECT : %d\n", lst->n_sect);
		
		lst = lst->next;
	}
}

void sort_sym_lst(t_sym **lst)
{
	uint8_t	sorted;
	t_sym	*sym;
	t_sym	tmp;
	int		diff;
   
	sorted = 0;
	if (!lst)
		return;
	while (!sorted)
	{
		sorted = 1;
		sym	= *lst;
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

void	add_sect_lst(void *seg, t_sect **sect_lst, uint8_t arch_64, uint8_t l_endian)
{
	uint32_t				nsect;
	char					*sectname;
	void					*section;
	uint8_t					n;
	t_sect					*sect;
	t_sect					*last_node;

	nsect = (arch_64 ? ((struct segment_command_64 *)seg)->nsects : ((struct segment_command *)seg)->nsects);
	nsect = l_endian ? swap_uint32(nsect) : nsect;
	section = ((void *)seg +
			(arch_64 ? sizeof(struct segment_command_64) : sizeof(struct segment_command)));
	n = 0;
	sect = *sect_lst;
	last_node = NULL;
	while (sect)
	{
		last_node = sect;
		sect = sect->next;
	}
	sect = last_node;
	//ft_printf("SEGNAME : %s\n", seg->segname);
	while (n < nsect)
	{
		//ft_printf("SECTION (%d) \n", n);
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
		sectname = (arch_64 ? ((struct section_64 *)section)[n].sectname : ((struct section *)section)[n].sectname);

		if (ft_strcmp(sectname, SECT_TEXT) == 0) //&&
			   //strcmp(section[n].segname, SEG_TEXT) == 0)
			sect->section = 'T';
		else if (ft_strcmp(sectname, SECT_DATA) == 0) //&&
			//strcmp(section[n].segname, SEG_DATA) == 0)
			sect->section = 'D';
		else if (ft_strcmp(sectname, SECT_BSS) == 0) //&&
			//strcmp(section[n].segname, SEG_DATA) == 0)
			sect->section = 'B';
		//else if (strcmp(section[n].sectname, SECT_COMMON) == 0) //&&
		//	//strcmp(section[n].segname, SEG_DATA) == 0)
		//	sect->section = 'C';
		n++;
	}
}

void	add_symtab_lst(int nsyms, int symoff, int stroff, char *ptr, t_sym **sym_lst, uint8_t arch_64, uint8_t l_endian)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;
	t_sym			*sym;
	t_sym			*last_node;

	(void)l_endian;
	i = 0;
	//ft_printf("STROFF %d\n", stroff);
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
		el = (void *)ptr + symoff + (i * (arch_64 ? sizeof(struct nlist_64) : sizeof(struct nlist)));
		el->n_un.n_strx =
			l_endian ? 
			swap_uint32(el->n_un.n_strx)
			: el->n_un.n_strx;

		el->n_desc = l_endian ? swap_uint16(el->n_desc) : el->n_desc;

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

			//ft_printf("SYM: %s\n", stringtable + el->n_un.n_strx);
			*sym = (t_sym){arch_64, arch_64 ?
				(l_endian ? swap_uint64(el->n_value) : el->n_value)
				: (l_endian ? (uint64_t)swap_uint32(((struct nlist *)el)->n_value) : ((struct nlist *)el)->n_value), 
				'?', el->n_sect, stringtable + el->n_un.n_strx, NULL};


			//if ((el->n_type & N_PEXT))
			//	ft_printf("N_PEXT\n");
		    //ft_printf("SYMBOL [%x] [%s] [%d]\n", (unsigned int)sym,stringtable + el->n_un.n_strx, el->n_sect);

			if ((el->n_type & N_TYPE) == N_UNDF)
			{
				sym->type = (el->n_sect == NO_SECT) ? 'U' : 'C';
			}
			else if ((el->n_type & N_TYPE) == N_ABS)
				sym->type = 'A';
			else if ((el->n_type & N_TYPE) == N_SECT)
			{
				sym->type = 'T';
				//sym->n_sect = el->n_sect;
			}
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
	ncmds = is_le(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = ((void *)ptr + (is_arch_64(mask) ? sizeof(struct mach_header_64) : sizeof(struct mach_header))), is_le(mask));
	i = 0;
	if (f)
		ft_printf("\n%s:\n", f);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT) {
			add_sect_lst(lc, sect_lst, is_arch_64(mask), is_le(mask));
		}
		else if (lc->cmd == LC_SYMTAB)
		{
			//printf("SYMTAB [%d]\n", i);
			sym = (struct symtab_command *)lc;

			add_symtab_lst(is_le(mask) ? swap_uint32(sym->nsyms) : sym->nsyms,
							is_le(mask) ? swap_uint32(sym->symoff) : sym->symoff,
							is_le(mask) ? swap_uint32(sym->stroff) : sym->stroff, ptr, sym_lst, is_arch_64(mask), is_le(mask));
			//ft_printf("LST ADDR: %#x\n", (unsigned int)sym_lst);
		}
		swap_load_command(lc = (void *)lc + lc->cmdsize, is_le(mask));
		i++;
	}
	sort_sym_lst(sym_lst);
	disp_sym_lst(*sym_lst, *sect_lst);
	free_lists(sym_lst, sect_lst);
}

void	otool_section(void *seg, uint8_t arch_64, uint8_t l_endian, char *ptr)
{
	uint32_t				nsect;
	char					*sectname;
	void					*section;
	uint8_t					n;

	nsect = (arch_64 ? ((struct segment_command_64 *)seg)->nsects : ((struct segment_command *)seg)->nsects);
	nsect = l_endian ? swap_uint32(nsect) : nsect;
	section = ((void *)seg +
			(arch_64 ? sizeof(struct segment_command_64) : sizeof(struct segment_command)));
	n = 0;
	while (n < nsect)
	{
		sectname = (arch_64 ? ((struct section_64 *)section)[n].sectname : ((struct section *)section)[n].sectname);

		if (ft_strcmp(sectname, SECT_TEXT) == 0)
		{
			int offset = (arch_64 ? ((struct section_64 *)section)[n].offset : ((struct section *)section)[n].offset);
			int size = (arch_64 ? ((struct section_64 *)section)[n].size : ((struct section *)section)[n].size);
			uint64_t addr = (arch_64 ? ((struct section_64 *)section)[n].addr : ((struct section *)section)[n].addr);
			hexdump(ptr + offset, size, addr);
		}
		n++;
	}
}

void	otool_macho(char *f, char *ptr, uint8_t mask)
{
	int							ncmds;
	int							i;
	struct mach_header 			*header;
	struct load_command			*lc;

	header = (struct mach_header *)ptr;
	ncmds = is_le(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = (void *)ptr + (is_arch_64(mask) ? sizeof(struct mach_header_64) : sizeof(struct mach_header)), is_le(mask));
	i = 0;
	if (f)
		ft_printf("\n%s:\n", f);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT) {
			otool_section(lc, is_arch_64(mask), is_le(mask), ptr);
		}
		swap_load_command(lc = (void *)lc + lc->cmdsize, is_le(mask));
		i++;
	}
}

void	handle_macho(char *f, char *ptr, uint8_t mask)
{
	if (nm(mask))
		nm_macho(f, ptr, mask);
	else
		otool_macho(f, ptr, mask);
}
