/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 16:29:18 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/09 12:14:07 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include "libft.h"
# include "swap.h"
# include "utils.h"
# include "masks.h"

# include <unistd.h>
# include <mach-o/loader.h>

# include <mach-o/fat.h>
# include <ar.h>
# include <sys/mman.h>
# include <sys/stat.h>

typedef struct		s_sym
{
	uint8_t			arch_64;
	uint64_t		value;
	char			type;
	uint8_t			n_sect;
	char			*name;
	struct s_sym	*next;
}					t_sym;

typedef struct		s_sect
{
	char			section;
	struct s_sect	*next;
}					t_sect;

/*
**  nm_otool.c
*/
void				disp_err(char *bin, char *f, char *err);
uint8_t				nm_otool(int fd, char *file, uint8_t disp, uint8_t mask);
uint8_t				handlers(char *f, char *ptr, uint8_t mask);

/*
**  fat.c
*/
void				handle_fat(char *f, char *ptr, uint8_t mask);

/*
**  ar.c
*/
void				handle_ar(char *f, char *ptr, uint8_t mask);

/*
**  macho.c
*/
void				handle_macho(char *f, char *ptr, uint8_t mask);

#endif
