#include "nm.h"
#include "swap.h"

void disp_err(char *f, char *err)
{
	ft_putstr_fd("nm: ", 2);
	ft_putstr_fd(f, 2);
	ft_putstr_fd(err, 2);
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

		if (strcmp(sectname, SECT_TEXT) == 0) //&&
			   //strcmp(section[n].segname, SEG_TEXT) == 0)
			sect->section = 'T';
		else if (strcmp(sectname, SECT_DATA) == 0) //&&
			//strcmp(section[n].segname, SEG_DATA) == 0)
			sect->section = 'D';
		else if (strcmp(sectname, SECT_BSS) == 0) //&&
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
		//el->n_un.n_strx = l_endian ? swap_uint32(el->n_un.n_strx) : el->n_un.n_strx;
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
			*sym = (t_sym){arch_64, arch_64 ? el->n_value : (uint64_t)((struct nlist *)el)->n_value, '?', el->n_sect, stringtable + el->n_un.n_strx, NULL};
				

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
			else if (el->n_type & N_INDR)
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


void handle_macho(char *f, char *ptr, uint8_t l_endian, uint8_t arch_64)
{
	int							ncmds;
	int							i;
	struct mach_header 			*header;
	struct load_command			*lc;
	struct symtab_command		*sym;
	t_sym						*sym_lst;
	t_sect						*sect_lst;

	header = (struct mach_header *)ptr;
	ncmds = header->ncmds;
	swap_load_command(lc = (void *)ptr + (arch_64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header)), l_endian);
	i = 0;
	sym_lst = NULL;
	sect_lst = NULL;
	if (f)
		ft_printf("\n%s:\n", f);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT) {
			add_sect_lst(lc, &sect_lst, arch_64, l_endian);
		}
		else if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			add_symtab_lst(sym->nsyms, sym->symoff, sym->stroff, ptr, &sym_lst, arch_64, l_endian);
			//ft_printf("LST ADDR: %#x\n", (unsigned int)sym_lst);
		}
		swap_load_command(lc = (void *)lc + lc->cmdsize, l_endian);
		i++;
	}
	sort_sym_lst(&sym_lst);
	disp_sym_lst(sym_lst, sect_lst);
}

void handle_ar(char *f, char *ptr)
{
	struct ar_hdr *ar_header;	
	unsigned int filename_pad;
	char 		*filename;

	ar_header = (struct ar_hdr *)(ptr + SARMAG);
	int ar_size = ft_atoi(ar_header->ar_size);
	//filename_pad = ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1));
	//uint32_t ranlibs_size = *((uint32_t *)((void *)ar_header + sizeof(struct ar_hdr) + filename_pad));
	//printf("RANLIBS SIZE : %d\n", ranlibs_size);
	//printf("SIZEOF RANLIB %lu\n", ranlibs_size / sizeof(struct ranlib_64));

	while ((ar_header = (struct ar_hdr *)((void*)ar_header + sizeof(struct ar_hdr) + ar_size))
		   && ((char *)ar_header)[0]) {

		
		ar_size = ft_atoi(ar_header->ar_size);
		printf("SIZE : (%d)\n", ar_size);

		if (ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		{
			filename_pad = ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1));
			filename = ar_header->ar_name + sizeof(struct ar_hdr);
			ft_printf("\n%s(%s):\n", f, filename);
			nm(NULL, 0, (char *)ar_header + sizeof(struct ar_hdr) + filename_pad);
		} else {
		
			filename = ar_header->ar_name;
			int i = 0;
			while (filename[i]) {
				if (filename[i] == 0x20)
				{
					filename[i] = 0;
					break ;
				}
				i++;
			}
			ft_printf("\n%s(%s):\n", f, filename);
			nm(NULL, 0, (char *)ar_header + sizeof(struct ar_hdr));
		}
	}
}

void handle_fat(char *f, char *ptr, uint8_t l_endian)
{
	uint32_t					i;
	struct fat_header			*header;
	struct fat_arch				*arch;

	swap_fat_header((header = (struct fat_header *)ptr), l_endian);
	swap_fat_arch(arch = (void *)ptr + sizeof(*header), header->nfat_arch, l_endian);
	i = 0;
	//ft_printf("NARCH : %u\n", header->nfat_arch);
	while (i < header->nfat_arch)
	{
		//ft_printf("[%d] ARCH : %#x\n", i, arch);
		//ft_printf("[%d] ARCH CPUTYPE : %u\n", i, arch->cputype);
		//ft_printf("[%d] ARCH SIZE : %u\n", i, arch->size);
		//ft_printf("[%d] ARCH OFFSET : %u\n", i, arch->offset);
		if (arch->cputype == CPU_TYPE_X86_64)
		{
			nm(f, 0, (void *)ptr + arch->offset);
			return;
		}
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
	if (header->nfat_arch > 0)
	{
		arch = (void *)ptr + sizeof(*header);
		nm(f, 0, (void *)ptr + arch->offset);
	}
}

void nm(char *f, uint8_t disp, char *ptr)
{
	unsigned int	magic_number;

	magic_number = *(unsigned int *)ptr;
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		handle_macho(disp ? f : NULL, ptr, (magic_number == MH_MAGIC) ? 0 : 1, 0);
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_macho(disp ? f : NULL, ptr, (magic_number == MH_MAGIC_64) ? 0 : 1, 1);
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(f, ptr, (magic_number == FAT_MAGIC) ? 0 : 1);
	else if (magic_number == *((unsigned int *)ARMAG))
		handle_ar(f, ptr);
	else
		//ft_printf("MAGIC NUMBER : %x\n", magic_number);
		disp_err(f, " The file was not recognized as a valid object file.\n");
}

int main(int argc, char **argv)
{
	int 		fd;
	char		*ptr;
	struct stat	buf;
	int			i;

	i = 0;
	while (++i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) < 0)
		{
			disp_err(argv[i], "Permission denied.\n");
			continue ;
		}
		if (fstat(fd, &buf) < 0)
			continue;
		if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			continue;

		nm(argv[i], argc > 2 ? 1 : 0, ptr);

		if (munmap(ptr, buf.st_size) < 0)
			continue;
		if (close(fd) == -1)
			continue;
	}
	return (0);
}
