/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:36:00 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/05 20:28:20 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SWAP_H
# define SWAP_H

# include <stdint.h>
# include <mach-o/fat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/arch.h>
# include <mach/machine.h>

uint64_t	swap_uint64(uint64_t x);
uint32_t	swap_uint32(uint32_t x);
uint16_t	swap_uint16(uint16_t x);
void		swap_fat_header(struct fat_header *fat_header,
							uint8_t l_endian);
void		swap_fat_arch(struct fat_arch *fat_archs,
							uint32_t nfat_arch,
							uint8_t l_endian);
void		swap_load_command(struct load_command *lc, uint8_t l_endian);

#endif
