/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/12 18:25:10 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/12 18:25:30 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <symbols.h>

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
