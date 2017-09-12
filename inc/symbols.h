/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 14:25:14 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/11 17:58:39 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYMBOLS_H
# define SYMBOLS_H

#include "libft.h"

#include "swap.h"
#include "masks.h"

# include <mach-o/loader.h>
# include <mach-o/nlist.h>

typedef struct		s_sect
{
	char			section;
	struct s_sect	*next;
}					t_sect;

typedef struct		s_sym
{
	uint8_t			arch_64;
	uint64_t		value;
	char			type;
	uint8_t			n_sect;
	char			*name;
	struct s_sym	*next;
}					t_sym;

typedef struct		s_add_sym
{
	int				i;
	char			*stringtable;
	struct nlist_64	*el;
	t_sym			*sym;
	int				nsyms;
}					t_add_sym;

void	free_lists(t_sym **sym_lst, t_sect **sect_lst);
void	disp_sym_lst(t_sym *lst, t_sect *sect_lst);
void	sort_sym_lst(t_sym **lst);
void	add_sect_lst(void *seg, t_sect **sect_lst, uint8_t mask);
void	add_symtab_lst(struct symtab_command *symtab, char *ptr,
						t_sym **sym_lst, uint8_t mask);

#endif
