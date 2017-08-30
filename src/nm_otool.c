#include "nm_otool.h"

void handlers(char *f, uint8_t disp, char *ptr, uint8_t mask)
{
	unsigned int	magic_number;

	magic_number = *(unsigned int *)ptr;
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		handle_macho(disp ? f : NULL, ptr, (magic_number == MH_MAGIC) ? 0 : 1, 0);
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
		handle_macho(disp ? f : NULL, ptr, (magic_number == MH_MAGIC_64) ? 0 : 1, 1);
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(f, ptr, mask | (((magic_number == FAT_MAGIC) ? LE : BE) << ENDIANNESS_MASK));
	else if (magic_number == *((unsigned int *)ARMAG))
		handle_ar(f, ptr);
	else
		disp_err(f, " The file was not recognized as a valid object file.\n");
		//ft_printf("MAGIC NUMBER : %x\n", magic_number);
}

uint8_t	nm_otool(int fd, char *file, uint8_t disp, uint8_t mask)
{
	char		*ptr;
	struct stat	buf;

	if (fstat(fd, &buf) < 0)
		return (-1);
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (-1);

	handlers(file, disp, ptr, mask);

	if (munmap(ptr, buf.st_size) < 0)
		return (-1);
	return (0);
}
