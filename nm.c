#include "nm.h"

void disp_sym_lst(t_sym *lst)
{
	while (lst)
	{
		if (lst->value)
			printf("%016llx", lst->value);
		else
			printf("                ");
		printf(" %c", lst->type);
		printf(" %s\n", lst->name);
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
				sym->name = sym->next->name;
				sym->next->value = tmp.value;
				sym->next->type = tmp.type;
				sym->next->name = tmp.name;
				sorted = 0;
			}
			sym = sym->next;
		}
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

		*sym = (t_sym){0, 0, stringtable + el[i].n_un.n_strx, NULL};

		if ((el[i].n_type & N_STAB))
			printf("N_STAB\n");

		if ((el[i].n_type & N_PEXT))
			printf("N_PEXT\n");

		if ((el[i].n_type & N_EXT))
			printf("N_EXT\n");

		if ((el[i].n_type & N_TYPE) == N_UNDF)
			sym->type = 'U';
		else if ((el[i].n_type & N_TYPE) == N_ABS)
			sym->type = 'A';
		else if ((el[i].n_type & N_TYPE) == N_SECT)
		{
			sym->value = el[i].n_value;
			sym->type = 'T';
		}
		else if ((el[i].n_type & N_TYPE) == N_PBUD)
			sym->type = 'U';
		//else if ((el[i].n_type & N_TYPE) == N_INDR)
		//	sym->type = '??????????';
		else
			sym->type = '?';

		i++;
	}
}

//struct mach_header_64 {
//        uint32_t        magic;          /* mach magic number identifier */
//        cpu_type_t      cputype;        /* cpu specifier */
//        cpu_subtype_t   cpusubtype;     /* machine specifier */
//        uint32_t        filetype;       /* type of file */
//        uint32_t        ncmds;          /* number of load commands */
//        uint32_t        sizeofcmds;     /* the size of all the load commands */
//        uint32_t        flags;          /* flags */
//        uint32_t        reserved;       /* reserved */
//};

void handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command 	*lc;
	struct symtab_command	*sym;
	t_sym					*sym_lst;
	struct segment_command_64       *seg;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	i = 0;
	sym_lst = NULL;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			struct section_64       *sect;
			int n = 0;

			seg = (struct segment_command_64 *)lc;
			printf("SEGNAME : %s\n", seg->segname);
			sect = (struct section_64 *)(seg + sizeof(*seg));
			while (n < seg->nsects)
			{
				  printf("SECTION\n");

				  printf("SEGNAME2 : %s\n", sect[i].segname);
				  printf("SECTNAME : %s\n", sect[i].sectname);
				  printf("SECT ADDR : %#llx\n", sect[i].addr);
				  n++;
			}
		}
		else if (lc->cmd == LC_SYMTAB)	
		{
			sym = (struct symtab_command *)lc;
			add_symtab_lst(sym->nsyms, sym->symoff, sym->stroff, ptr, &sym_lst);
			printf("LST ADDR: %#x\n", (unsigned int)sym_lst);
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	sort_sym_lst(&sym_lst);
	disp_sym_lst(sym_lst);
}

void nm(char *ptr)
{
	int magic_number;
	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
}

int main(int argc, char **argv)
{
	int 		fd;
	char		*ptr;
	struct stat buf;

	fflush(stdout);
	if (argc != 2)
		return (EXIT_FAILURE);
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		return (EXIT_FAILURE);
	if (fstat(fd, &buf) < 0)
		return (EXIT_FAILURE);
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (EXIT_FAILURE);

	nm(ptr);

	if (munmap(ptr, buf.st_size) < 0)
		return (EXIT_FAILURE);
	return (0);
}
