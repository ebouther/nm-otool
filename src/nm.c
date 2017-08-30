#include "nm.h"

int main(int argc, char **argv)
{
	int 		fd;
	int			i;

	i = 0;
	while (++i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) < 0)
		{
			disp_err(argv[i], "Permission denied.\n");
			continue ;
		}
		nm_otool(fd, argv[i], argc > 2 ? 1 : 0, NM);

		if (close(fd) == -1)
			continue;
	}
	return (0);
}
