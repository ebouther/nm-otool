#include "nm.h"

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
			if (lst->type != 'U')
				ft_printf("%016llx", lst->value);
			else
				ft_printf("                ");
			ft_printf(" %c", lst->type);
			//ft_printf(" %d", lst->n_sect); // DBG
			ft_printf(" %s\n", lst->name);
			//ft_printf(" N_SECT : %d\n", lst->n_sect);
		
		lst = lst->next;
	}
}

void sort_sym_lst(t_sym **lst)//, uint8_t sort_by)
{
	uint8_t	sorted;
	t_sym	*sym;
	t_sym	tmp;
   
	sorted = 0;
	if (!lst)
		return;
	while (!sorted)
	{
		sorted = 1;
		sym	= *lst;
		while (sym && sym->next)
		{
			if (strcmp(sym->name, sym->next->name) > 0)
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

void	add_sect_lst(struct segment_command_64 *seg, t_sect **sect_lst)
{

	struct section_64		*section;
	uint8_t					n;
	t_sect					*sect;
	t_sect					*last_node;

	section = (struct section_64 *)((void *)seg + sizeof(struct segment_command_64));
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
	//ft_printf("SECTNAME : %s", section[n].sectname);
	while (n < seg->nsects)
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

		if (strcmp(section[n].sectname, SECT_TEXT) == 0) //&&
			   //strcmp(section[n].segname, SEG_TEXT) == 0)
			sect->section = 'T';
		else if (strcmp(section[n].sectname, SECT_DATA) == 0) //&&
			//strcmp(section[n].segname, SEG_DATA) == 0)
			sect->section = 'D';
		else if (strcmp(section[n].sectname, SECT_BSS) == 0) //&&
			//strcmp(section[n].segname, SEG_DATA) == 0)
			sect->section = 'B';
		//else if (strcmp(section[n].sectname, SECT_COMMON) == 0) //&&
		//	//strcmp(section[n].segname, SEG_DATA) == 0)
		//	sect->section = 'C';
		n++;
	}

}

void	add_symtab_lst(int nsyms, int symoff, int stroff, char *ptr, t_sym **sym_lst)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;
	t_sym			*sym;
	t_sym			*last_node;

	el = (void *)ptr + symoff;
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
		if (ft_strlen(stringtable + el[i].n_un.n_strx) > 0
			&& !(el[i].n_type & N_STAB))
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

			//ft_printf("SYM: %s\n", stringtable + el[i].n_un.n_strx);
			*sym = (t_sym){0, 0, el[i].n_sect, stringtable + el[i].n_un.n_strx, NULL};

			//if ((el[i].n_type & N_PEXT))
			//	ft_printf("N_PEXT\n");
		    //ft_printf("SYMBOL [%x] [%s] [%d]\n", (unsigned int)sym,stringtable + el[i].n_un.n_strx, el[i].n_sect);

			if ((el[i].n_type & N_TYPE) == N_UNDF)
			{
				sym->type = (el[i].n_sect == NO_SECT) ? 'U' : 'C';
			}
			else if ((el[i].n_type & N_TYPE) == N_ABS)
				sym->type = 'A';
			else if ((el[i].n_type & N_TYPE) == N_SECT)
			{
				sym->value = el[i].n_value;
				sym->type = 'T';
				//sym->n_sect = el[i].n_sect;
			}
			else if ((el[i].n_type & N_TYPE) == N_PBUD)
				sym->type = 'U';
			else if (el[i].n_type & N_INDR)
				sym->type = 'I';
			else
				sym->type = '?';
			if (!(el[i].n_type & N_EXT)
					&& sym->type >= 'A'
					&& sym->type <= 'Z')
				sym->type += 32;
				
		}
		i++;
	}
}


void handle_64(char *ptr)
{
	int							ncmds;
	int							i;
	struct mach_header_64		*header;
	struct load_command			*lc;
	struct symtab_command		*sym;
	struct segment_command_64	*seg;
	t_sym						*sym_lst;
	t_sect						*sect_lst;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	i = 0;
	sym_lst = NULL;
	sect_lst = NULL;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			add_sect_lst((struct segment_command_64 *)lc, &sect_lst);
		else if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			add_symtab_lst(sym->nsyms, sym->symoff, sym->stroff, ptr, &sym_lst);
			//ft_printf("LST ADDR: %#x\n", (unsigned int)sym_lst);
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	sort_sym_lst(&sym_lst);
	disp_sym_lst(sym_lst, sect_lst);
}

void handle_fat(char *ptr)
{
	int							nfat_arch;
	int							i;
	struct fat_header			*header;
	struct fat_arch				*arch;

	header = (struct fat_header *)ptr;
	nfat_arch = header->nfat_arch;
	arch = (void *)ptr + sizeof(*header);
	i = 0;
	while (i < nfat_arch)
	{
		nm((char *)((void *)ptr + arch->offset));
		arch = (void *)arch + arch->size;
		i++;
	}
}

void nm(char *ptr)
{
	int magic_number;
	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_64(ptr);
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(ptr);
}

int main(int argc, char **argv)
{
	int 		fd;
	char		*ptr;
	struct stat	buf;
	int			i;

	i = 1;
	while (i < argc)
	{
		if (argc > 2)
			ft_printf("\n%s:\n", argv[i]);
		if ((fd = open(argv[i], O_RDONLY)) < 0)
			return (EXIT_FAILURE);
		if (fstat(fd, &buf) < 0)
			return (EXIT_FAILURE);
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			return (EXIT_FAILURE);

		nm(ptr);

		if (munmap(ptr, buf.st_size) < 0)
			return (EXIT_FAILURE);
		if (close(fd) == -1)
			return (EXIT_FAILURE);
		i++;
	}
	fflush(stdout);
	return (0);
}
