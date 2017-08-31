#include "nm.h"

int main(int argc, char **argv)
{
	int 		fd;
	int			i;
	char		*file;

	i = 0;
	while (++i < argc || argc == 1)
	{
		if (argc == 1)
		{
			file = ft_strcpy(ft_strnew(5), "a.out");
			argc = 0;
		}
		else
			file = argv[i];
		if ((fd = open(file, O_RDONLY)) < 0)
		{
			disp_err(file, "Permission denied.\n");
			continue ;
		}
		nm_otool(fd, file, argc > 2 ? 1 : 0, NM);

		if (close(fd) == -1)
			continue;
	}
	return (0);
}
