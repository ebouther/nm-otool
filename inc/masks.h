#ifndef MASKS_H
# define MASKS_H

# define NM_OTOOL_MASK 0x01
# define OTOOL 0x00
# define NM 0x01

# define ENDIANNESS_BIT 1 
# define ENDIANNESS_MASK 0x02
# define BE 0x00
# define LE 0x01

# define le(mask) (mask | (1 << ENDIANNESS_BIT))
# define be(mask) (mask & ~(1 << ENDIANNESS_BIT))
# define is_le(mask) ((mask & ENDIANNESS_MASK) != 0 ? 1 : 0)
# define is_be(mask) (!is_le(mask)) 

# define ARCH_64_BIT 2 
# define ARCH_64_MASK 0x04
# define ARCH_32 0x00
# define ARCH_64 0x01

# define arch_64(mask) (mask | (1 << ARCH_64_BIT))
# define arch_32(mask) (mask & ~(1 << ARCH_64_BIT))
# define is_arch_64(mask) ((mask & ARCH_64_MASK) != 0 ? 1 : 0)
# define is_arch_32(mask) (!is_arch_64(mask)) 

# define nm(mask) (mask & NM_OTOOL_MASK)

#endif
