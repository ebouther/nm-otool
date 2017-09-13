/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 18:25:10 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/13 14:04:29 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <symbols.h>

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

void	disp_sym(t_sym *lst)
{
	if ((FLAG == FLAG_UPPER_U
			&& lst->type == 'U')
		|| (FLAG == FLAG_U
			&& lst->type != 'U'))
		return ;
	if (lst->type != 'U')
	{
		if (FORMAT == NO_FLAG)
			ft_printf(lst->arch_64 ? "%016lx " : "%08lx ", lst->value);
		else if (FORMAT == F_DEC)
			ft_printf(lst->arch_64 ? "%016ld " : "%08ld ", lst->value);
		else if (FORMAT == F_OCT)
			ft_printf(lst->arch_64 ? "%016lo " : "%08lo ", lst->value);
	}
	else if (FLAG != FLAG_U)
		ft_printf(lst->arch_64 ? "                 " : "         ");
	if (FLAG != FLAG_U)
		ft_printf("%c ", lst->type);
	ft_printf("%s\n", lst->name);
}

void	disp_sym_lst(t_sym *lst, t_sect *sect_lst)
{
	t_sect	*begin;

	begin = sect_lst;
	while (lst)
	{
		sect_lst = begin;
		if (lst->n_sect && sect_lst && (lst->type == 'T' || lst->type == 't')
				&& lst->n_sect != NO_SECT)
			get_sym_sect(sect_lst, lst);
		if (FLAG == FLAG_J)
			ft_printf("%s\n", lst->name);
		else
			disp_sym(lst);
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
	while (lst && !sorted)
	{
		sorted = 1;
		sym = *lst;
		while (sym && sym->next)
		{
			if ((FLAG != FLAG_R
				&& ((diff = ft_strcmp(sym->name, sym->next->name)) > 0
					|| (diff == 0 && sym->value > sym->next->value))) ||
				(FLAG == FLAG_R
				&& ((diff = ft_strcmp(sym->next->name, sym->name)) > 0
					|| (diff == 0 && sym->next->value > sym->value))))
			{
				swap_sym_node(sym);
				sorted = 0;
			}
			sym = sym->next;
		}
	}
}
