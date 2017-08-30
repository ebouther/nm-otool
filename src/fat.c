#include "nm_otool.h"

void handle_fat(char *f, char *ptr, uint8_t mask)
{
	uint32_t					i;
	struct fat_header			*header;
	struct fat_arch				*arch;

	swap_fat_header((header = (struct fat_header *)ptr), mask & ENDIANNESS_MASK);
	swap_fat_arch(arch = (void *)ptr + sizeof(*header), header->nfat_arch, mask & ENDIANNESS_MASK);
	i = 0;
	while (i < header->nfat_arch)
	{
		if (arch->cputype == CPU_TYPE_X86_64)
		{
			handlers(f, 0, (void *)ptr + arch->offset, mask);
			return;
		}
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
	i = 0;
	arch = (void *)ptr + sizeof(*header);
	while (i < header->nfat_arch)
	{
		if (header->nfat_arch > 1)
			ft_printf("\n%s (for architecture %s):\n", f, NXGetArchInfoFromCpuType(arch->cputype, arch->cpusubtype)->name);
		else
			ft_printf("%s:\n", f);
		handlers(f, 0, (void *)ptr + arch->offset, mask);
		arch = (void *)arch + sizeof(*arch);
		i++;
	}
}
