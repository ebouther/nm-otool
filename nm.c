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
   
	sorted = 1;
	while (sorted)
	{
		sorted = 0;
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
				sorted = 1;
			}
			sym = sym->next;
		}
	}
}

t_sym *get_symtab_lst(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;
	t_sym			*sym;
	t_sym			*sym_lst;

	el = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;
	sym = NULL;
	sym_lst = NULL;
	while (i < nsyms)
	{
		if (sym == NULL)
		{
			sym = (t_sym *)malloc(sizeof(t_sym));
			sym_lst = sym;
		}
		else
		{
			sym->next = (t_sym *)malloc(sizeof(t_sym));
			sym = sym->next;
		}
		*sym = (t_sym){0, 0, stringtable + el[i].n_un.n_strx, NULL};
		if ((el[i].n_type & N_TYPE) == N_UNDF)
			sym->type = 'U';
		else if ((el[i].n_type & N_TYPE) == N_ABS)
			sym->type = 'A';
		else if ((el[i].n_type & N_TYPE) == N_SECT)
		{
			sym->value = el[i].n_value;
			sym->type = 'T';
			printf("N_SECT : %d\n", el[i].n_sect);
		}
		else if ((el[i].n_type & N_TYPE) == N_PBUD)
			sym->type = 'U';
		//else if ((el[i].n_type & N_TYPE) == N_INDR)
		//	sym->type = '??????????';
		else
			sym->type = '?';
		i++;
	}
	return (sym_lst);
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

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)	
		{
			sym = (struct symtab_command *)lc;
			sym_lst = get_symtab_lst(sym->nsyms, sym->symoff, sym->stroff, ptr);
			sort_sym_lst(&sym_lst);
			disp_sym_lst(sym_lst);
			break;
		}
		else if (lc->cmd == LC_SYMTAB)	
		{
			sym = (struct dysymtab_command *)lc;
			sym_lst = get_sym_lst(sym->nsyms, sym->symoff, sym->stroff, ptr);
			sort_sym_lst(&sym_lst);
			disp_sym_lst(sym_lst);
			break;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
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
