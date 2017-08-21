/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/15 15:22:44 by ebouther          #+#    #+#             */
/*   Updated: 2017/06/07 18:15:29 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <ar.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>

#                          include <stdlib.h> // remove norme
#                          include <string.h>
# include "libft.h"

typedef struct 		s_sym
{
	uint8_t			arch_64;
	uint64_t		value;
	char			type;
	uint8_t			n_sect;
	char			*name;
	struct s_sym	*next;
}					t_sym;

typedef struct 		s_sect
{
	char			section;
	struct s_sect	*next;
}					t_sect;

void nm(char *f, uint8_t disp, char *ptr);

#endif
