/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   masks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 20:15:57 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/12 15:27:45 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASKS_H
# define MASKS_H

# define NM_OTOOL_MASK 0x01
# define OTOOL 0x00
# define NM 0x01

# define ENDIANNESS_BIT 1
# define ENDIANNESS_MASK 0x02
# define BE 0x00
# define LE 0x01
# define SET_LE(mask) (mask | (1 << ENDIANNESS_BIT))
# define SET_BE(mask) (mask & ~(1 << ENDIANNESS_BIT))
# define IS_LE(mask) ((mask & ENDIANNESS_MASK) != 0 ? 1 : 0)
# define IS_BE(mask) (!IS_LE(mask))
# define ARCH_BIT 2
# define ARCH_64_MASK 0x04
# define ARCH_32 0x00
# define ARCH_64 0x01

# define SET_ARCH_64(mask) (mask | (1 << ARCH_BIT))
# define SET_ARCH_32(mask) (mask & ~(1 << ARCH_BIT))
# define IS_ARCH_64(mask) ((mask & ARCH_64_MASK) != 0 ? 1 : 0)
# define IS_ARCH_32(mask) (!IS_ARCH_64(mask))

# define DISP_BIT 3
# define DISP_MASK 0x08
# define HIDE 0x00
# define DISP 0x01

# define SET_DISP(mask) (mask | (1 << DISP_BIT))
# define SET_HIDE(mask) (mask & ~(1 << DISP_BIT))
# define IS_DISP(mask) ((mask & DISP_MASK) != 0 ? 1 : 0)
# define IS_HIDDEN(mask) (!IS_DISP(mask))

# define DUMP_FORMAT_BIT 4
# define DUMP_FORMAT_MASK 0x10

# define DUMP_4(mask) (mask | (1 << DUMP_FORMAT_BIT))
# define DUMP_1(mask) (mask & ~(1 << DUMP_FORMAT_BIT))

# define IS_DUMP_4(mask) ((mask & DUMP_FORMAT_MASK) != 0 ? 1 : 0)
# define IS_DUMP_1(mask) (!IS_DUMP_4(mask))

# define IS_NM(mask) (mask & NM_OTOOL_MASK)

#endif
