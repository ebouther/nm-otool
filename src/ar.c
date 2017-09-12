/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:29 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/12 17:39:25 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	get_macho_long_filename(char *f,
		struct ar_hdr *ar_header, uint8_t mask)
{
	unsigned int	filename_pad;
	char			*filename;

	filename_pad = ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1));
	filename = ar_header->ar_name + sizeof(struct ar_hdr);
	if (f)
		ft_printf("%s(%s):\n", f, filename);
	handlers(NULL, (char *)ar_header
			+ sizeof(struct ar_hdr) + filename_pad, mask);
}

static void	get_macho(char *f, struct ar_hdr *ar_header, uint8_t mask)
{
	char	*filename;
	int		i;

	i = 0;
	filename = ar_header->ar_name;
	while (filename[i])
	{
		if (filename[i] == 0x20)
		{
			filename[i] = 0;
			break ;
		}
		i++;
	}
	if (f)
		ft_printf("%s(%s):\n", f, filename);
	handlers(NULL, (char *)ar_header + sizeof(struct ar_hdr), mask);
}

int8_t		is_valid_ar(struct ar_hdr *ar_header)
{
	if (!(ft_strcmp((char *)ar_header + sizeof(struct ar_hdr), SYMDEF) == 0
		|| ft_strcmp((char *)ar_header + sizeof(struct ar_hdr), SYMDEF_64) == 0
		|| ft_strcmp((char *)ar_header + sizeof(struct ar_hdr), SYMDEF_SORTED)
		== 0 || ft_strcmp((char *)ar_header + sizeof(struct ar_hdr),
			SYMDEF_64_SORTED) == 0))
		return (-1);
	return (1);
}

void		handle_ar(char *f, char *ptr, uint8_t mask)
{
	struct ar_hdr	*ar_header;
	int				i;

	i = 0;
	ar_header = (struct ar_hdr *)(ptr + SARMAG);
	if (!is_valid_ar(ar_header))
		return ;
	if (IS_NM(mask))
		ft_putchar('\n');
	while ((ar_header = (struct ar_hdr *)
			((void*)ar_header + sizeof(struct ar_hdr)
			+ ft_atoi(ar_header->ar_size)))
			&& ((char *)ar_header)[0])
	{
		if (i == 0 && !IS_NM(mask))
			ft_printf("Archive : %s\n", f);
		if (i != 0 && IS_NM(mask))
			ft_putchar('\n');
		if (ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
			get_macho_long_filename(f, ar_header, mask);
		else
			get_macho(f, ar_header, mask);
		i++;
	}
}
