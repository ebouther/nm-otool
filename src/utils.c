#include "utils.h"

void disp_err(char *f, char *err)
{
	ft_putstr_fd("nm: ", 2);
	ft_putstr_fd(f, 2);
	ft_putstr_fd(err, 2);
}

void hexdump(void *data, size_t size, uint64_t addr)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < size)
	{
		ft_printf("%016llx\t", addr + i);
		j = 0;
		while (j < 16
				&& i + j < size)
		{
			ft_printf("%02x ", ((unsigned char *)data)[i + j]);
			j++;
		}
		ft_putchar('\n');
		i += 16;
	}
}
