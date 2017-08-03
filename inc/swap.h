/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:36:00 by ebouther          #+#    #+#             */
/*   Updated: 2017/06/07 18:15:39 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SWAP_H
# define SWAP_H

# include <stdint.h>
# include <mach-o/fat.h>
# include <mach/machine.h>

# define ARCH_64 1
# define ARCH_32 0

	uint32_t	swap_uint32(uint32_t x);
	void swap_fat_header(struct fat_header *fat_header, uint8_t l_endian);
	void swap_fat_arch(struct fat_arch *fat_archs, uint32_t nfat_arch,uint8_t l_endian);

#endif