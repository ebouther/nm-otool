/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:01:29 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/09 12:14:28 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	get_macho_long_filename(char *f, struct ar_hdr *ar_header, uint8_t mask)
{
	unsigned int 	filename_pad;
	char 			*filename;

	filename_pad = ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1));
	filename = ar_header->ar_name + sizeof(struct ar_hdr);
	ft_printf("\n%s(%s):\n", f, filename);
	handlers(NULL, (char *)ar_header + sizeof(struct ar_hdr) + filename_pad, mask);
}

static void	get_macho(char *f, struct ar_hdr *ar_header, uint8_t mask)
{
	char *filename;
	int	 i;

	i = 0;
	filename = ar_header->ar_name;
	while (filename[i]) {
		if (filename[i] == 0x20)
		{
			filename[i] = 0;
			break ;
		}
		i++;
	}
	ft_printf("\n%s(%s):\n", f, filename);
	handlers(NULL, (char *)ar_header + sizeof(struct ar_hdr), mask);
}

void	handle_ar(char *f, char *ptr, uint8_t mask)
{
	struct ar_hdr	*ar_header;	
	int				ar_size;

	ar_header = (struct ar_hdr *)(ptr + SARMAG);
	ar_size = ft_atoi(ar_header->ar_size);
	while ((ar_header = (struct ar_hdr *)((void*)ar_header + sizeof(struct ar_hdr) + ar_size))
		   && ((char *)ar_header)[0])
	{
		ar_size = ft_atoi(ar_header->ar_size);
		if (ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
			get_macho_long_filename(f, ar_header, mask);
		else
			get_macho(f, ar_header, mask);
	}
}
