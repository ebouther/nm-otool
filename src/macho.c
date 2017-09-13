/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:37 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/13 19:25:42 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int		parse_lc(t_sym **sym_lst, t_sect **sect_lst, char *ptr, uint8_t mask)
{
	int						i;
	int						ncmds;
	struct load_command		*lc;
	struct mach_header		*header;

	header = (struct mach_header *)ptr;
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	swap_load_command(lc = ((void *)ptr +
		(IS_ARCH_64(mask) ? sizeof(struct mach_header_64)
		: sizeof(struct mach_header))), IS_BE(mask));
	ncmds = IS_BE(mask) ? swap_uint32(header->ncmds) : header->ncmds;
	i = 0;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
			add_sect_lst(lc, sect_lst, mask);
		else if (lc->cmd == LC_SYMTAB)
			if (add_symtab_lst((struct symtab_command *)lc,
					ptr, sym_lst, mask) == -1)
				return (EXIT_FAILURE);
		swap_load_command(lc = (void *)lc + lc->cmdsize, IS_BE(mask));
		i++;
	}
	return (0);
}

int		nm_macho(char *f, char *ptr, uint8_t mask)
{
	t_sym						**sym_lst;
	t_sect						**sect_lst;

	sym_lst = malloc(sizeof(t_sym *));
	sect_lst = malloc(sizeof(t_sect *));
	*sym_lst = NULL;
	*sect_lst = NULL;
	if (f)
		ft_printf("\n%s:\n", f);
	if (parse_lc(sym_lst, sect_lst, ptr, mask) == -1)
		return (EXIT_FAILURE);
	sort_sym_lst(sym_lst);
	disp_sym_lst(*sym_lst, *sect_lst);
	free_lists(sym_lst, sect_lst);
	return (EXIT_SUCCESS);
}

int		handle_macho(char *f, char *ptr, uint8_t mask)
{
	if (IS_NM(mask))
		return (nm_macho(IS_HIDDEN(mask) ? NULL : f, ptr, mask));
	else
		otool_macho(f, ptr, mask);
	return (EXIT_SUCCESS);
}
