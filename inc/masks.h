#ifndef MASKS_H
# define MASKS_H

# define NM_OTOOL_MASK 0x01
# define NM 0x00
# define OTOOL 0x01

# define ENDIANNESS_BIT 1 
# define ENDIANNESS_MASK 0x02
# define BE 0x00
# define LE 0x01

# define set_byte_sex(mask, val) (mask | (val << ENDIANNESS_BIT))
# define l_endian(mask) (mask & ENDIANNESS_MASK)


# define ARCH_64_BIT 2 
# define ARCH_64_MASK 0x04
# define ARCH_32 0x00
# define ARCH_64 0x01

# define set_arch_type(mask, val) (mask | (val << ARCH_64_BIT))
# define arch_64(mask) (mask & ARCH_64_MASK)

#endif
