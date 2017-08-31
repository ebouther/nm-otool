#include "utils.h"

void disp_err(char *f, char *err)
{
	ft_putstr_fd("nm: ", 2);
	ft_putstr_fd(f, 2);
	ft_putstr_fd(err, 2);
}

void hexdump(void *data, size_t size, uint64_t addr, uint8_t mask)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < size)
	{
		is_arch_64(mask) ? ft_printf("%016llx\t", addr + i) : ft_printf("%08llx\t", addr + i);
		j = 0;
		while (j < 16
				&& i + j < size)
		{
			ft_printf("%02x", ((uint8_t *)data)[i + j++]);
			if (is_dump_1(mask) || (is_dump_4(mask) && j % 4 == 0))
				ft_putchar(' ');
		}
		ft_putchar('\n');
		i += 16;
	}
}
