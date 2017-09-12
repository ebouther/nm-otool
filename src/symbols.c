/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 14:24:18 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/12 15:51:19 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "symbols.h"

void	free_lists(t_sym **sym_lst, t_sect **sect_lst)
{
	t_sym		*lst;
	t_sect		*s_lst;
	void		*tmp;

	lst = *sym_lst;
	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
	s_lst = *sect_lst;
	while (s_lst)
	{
		tmp = s_lst;
		s_lst = s_lst->next;
		free(tmp);
	}
	free(sym_lst);
	free(sect_lst);
}

void	get_sym_sect(t_sect *sect_lst, t_sym *lst)
{
	int		i;

	i = 1;
	while (sect_lst)
	{
		if (i >= lst->n_sect)
			break ;
		sect_lst = sect_lst->next;
		i++;
	}
	if (i == lst->n_sect)
	{
		lst->type = sect_lst->section +
			((lst->type == 't') ? 32 : 0);
	}
}

#include <stdio.h>
void	disp_sym_lst(t_sym *lst, t_sect *sect_lst)
{
	t_sect	*begin;

	begin = sect_lst;
	while (lst)
	{
		sect_lst = begin;
		if (lst->n_sect && sect_lst && (lst->type == 'T' || lst->type == 't')
			&& lst->n_sect != NO_SECT)
		{
			get_sym_sect(sect_lst, lst);
		}
		if (lst->type != 'U')
			ft_printf(lst->arch_64 ? "%016lx " : "%08lx ", lst->value);
		else
			ft_printf(lst->arch_64 ? "                 " : "         ");
		ft_printf("%c ", lst->type);
		ft_printf("%s\n", lst->name);
		lst = lst->next;
	}
}

void	swap_sym_node(t_sym *sym)
{
	t_sym	tmp;

	tmp = *sym;
	sym->value = sym->next->value;
	sym->type = sym->next->type;
	sym->n_sect = sym->next->n_sect;
	sym->name = sym->next->name;
	sym->next->value = tmp.value;
	sym->next->type = tmp.type;
	sym->next->n_sect = tmp.n_sect;
	sym->next->name = tmp.name;
}

void	sort_sym_lst(t_sym **lst)
{
	uint8_t	sorted;
	t_sym	*sym;
	int		diff;

	sorted = 0;
	if (!lst)
		return ;
	while (!sorted)
	{
		sorted = 1;
		sym = *lst;
		while (sym && sym->next)
		{
			if ((diff = ft_strcmp(sym->name, sym->next->name)) > 0
				|| (diff == 0 && sym->value > sym->next->value))
			{
				swap_sym_node(sym);
				sorted = 0;
			}
			sym = sym->next;
		}
	}
}

t_sect	*last_node_sect(t_sect *sect)
{
	t_sect			*last_node;

	last_node = NULL;
	while (sect)
	{
		last_node = sect;
		sect = sect->next;
	}
	return (last_node);
}

void	set_sect(t_sect *sect, void *seg, uint8_t n, uint8_t mask)
{
	void					*section;
	char					*sectname;
	char					*segname;

	section = ((void *)seg +
			(IS_ARCH_64(mask) ? sizeof(struct segment_command_64)
			: sizeof(struct segment_command)));
	sectname = (IS_ARCH_64(mask) ? ((struct section_64 *)section)[n].sectname
			: ((struct section *)section)[n].sectname);
	segname = (IS_ARCH_64(mask) ? ((struct section_64 *)section)[n].segname
			: ((struct section *)section)[n].segname);
	if (ft_strcmp(sectname, SECT_TEXT) == 0)
		sect->section = 'T';
	else if (ft_strcmp(sectname, SECT_DATA) == 0)
		sect->section = 'D';
	else if (ft_strcmp(sectname, SECT_BSS) == 0)
		sect->section = 'B';
}

void	add_sect_lst(void *seg, t_sect **sect_lst, uint8_t mask)
{
	uint32_t				nsect;
	uint8_t					n;
	t_sect					*sect;

	nsect = (IS_ARCH_64(mask) ? ((struct segment_command_64 *)seg)->nsects
			: ((struct segment_command *)seg)->nsects);
	nsect = IS_BE(mask) ? swap_uint32(nsect) : nsect;
	n = 0;
	sect = last_node_sect(*sect_lst);
	while (n < nsect)
	{
		if (sect == NULL)
		{
			sect = (t_sect *)malloc(sizeof(t_sect));
			*sect_lst = sect;
		}
		else
		{
			sect->next = (t_sect *)malloc(sizeof(t_sect));
			sect = sect->next;
		}
		*sect = (t_sect){'S', NULL};
		set_sect(sect, seg, n, mask);
		n++;
	}
}

void	get_sym_type(struct nlist_64 *el, t_sym *sym)
{
	if ((el->n_type & N_TYPE) == N_UNDF)
	{
		sym->type = (el->n_sect == NO_SECT) ? 'U' : 'C';
		if (sym->value != 0)
			sym->type = 'C';
	}
	else if ((el->n_type & N_TYPE) == N_ABS)
		sym->type = 'A';
	else if ((el->n_type & N_TYPE) == N_SECT)
		sym->type = 'T';
	else if ((el->n_type & N_TYPE) == N_PBUD)
		sym->type = 'U';
	else if ((el->n_type & N_TYPE) == N_INDR)
		sym->type = 'I';
	else
		sym->type = '?';
	if (!(el->n_type & N_EXT)
			&& sym->type >= 'A'
			&& sym->type <= 'Z')
		sym->type += 32;
}

t_sym	*last_node_sym(t_sym *sym)
{
	t_sym			*last_node;

	last_node = NULL;
	while (sym)
	{
		last_node = sym;
		sym = sym->next;
	}
	return (last_node);
}

void	alloc_sym(t_sym **sym, t_sym **sym_lst)
{
	if (*sym == NULL)
	{
		*sym = (t_sym *)malloc(sizeof(t_sym));
		*sym_lst = *sym;
	}
	else
	{
		(*sym)->next = (t_sym *)malloc(sizeof(t_sym));
		*sym = (*sym)->next;
	}
}

void	set_sym(t_sym *sym, struct nlist_64 *el, uint8_t mask)
{
	if (IS_ARCH_64(mask))
		sym->value = IS_BE(mask) ? swap_uint64(el->n_value) : el->n_value;
	else
		sym->value = IS_BE(mask) ? (uint64_t)swap_uint32(
		((struct nlist *)el)->n_value) : ((struct nlist *)el)->n_value;
}

void	add_symtab_lst(struct symtab_command *symtab, char *ptr,
						t_sym **sym_lst, uint8_t mask)
{
	t_add_sym	a;

	a.i = 0;
	a.stringtable = (void *)ptr +
		(IS_BE(mask) ? swap_uint32(symtab->stroff) : symtab->stroff);
	a.sym = last_node_sym(*sym_lst);
	a.nsyms = IS_BE(mask) ? swap_uint32(symtab->nsyms) : symtab->nsyms;
	while (a.i < a.nsyms)
	{
		swap_nlist(a.el = (void *)ptr +
			(IS_BE(mask) ? swap_uint32(symtab->symoff) : symtab->symoff)
			+ (a.i * (IS_ARCH_64(mask) ?
				sizeof(struct nlist_64) : sizeof(struct nlist))), IS_BE(mask));
		if (ft_strlen(a.stringtable + a.el->n_un.n_strx) > 0
			&& !(a.el->n_type & N_STAB))
		{
			alloc_sym(&a.sym, sym_lst);
			*(a.sym) = (t_sym){IS_ARCH_64(mask), 0, '?',
				(a.el)->n_sect, a.stringtable + (a.el)->n_un.n_strx, NULL};
			set_sym(a.sym, a.el, mask);
			get_sym_type(a.el, a.sym);
		}
		a.i++;
	}
}
