/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 14:24:18 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/13 18:47:14 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "symbols.h"

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

int		add_symtab_lst(struct symtab_command *symtab, char *ptr,
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
		if (swap_nlist(a.el = (void *)ptr +
			(IS_BE(mask) ? swap_uint32(symtab->symoff) : symtab->symoff)
			+ (a.i * (IS_ARCH_64(mask) ? sizeof(struct nlist_64)
				: sizeof(struct nlist))), IS_BE(mask)) == -1)
			return (EXIT_FAILURE);
		if (strlen_secure(a.stringtable + a.el->n_un.n_strx) > 0
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
	return (EXIT_SUCCESS);
}
