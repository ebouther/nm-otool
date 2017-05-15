#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;

	el = (void *)ptr + symoff;
	stringtable = (void *)ptr + stroff;

	while (i < nsyms)
	{
		if ((el[i].n_type & N_TYPE) == N_UNDF)
			printf("U");
		else if ((el[i].n_type & N_TYPE) == N_SECT)
		{
			printf("%016llx ", el[i].n_value);
			printf("T");
		}
		else
			printf("%d  ", el[i].n_type);
		printf(" %s\n", stringtable + el[i].n_un.n_strx);
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

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	for (i = 0; i < ncmds; ++i)
	{
		if (lc->cmd == LC_SYMTAB)	
		{
			sym = (struct symtab_command *)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break;
		}
		lc = (void *)lc + lc->cmdsize;
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
