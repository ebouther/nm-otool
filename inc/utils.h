#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "masks.h"

void	disp_err(char *f, char *err);
void 	hexdump(void *data, size_t size, uint64_t addr, uint8_t arch_64);

#endif
