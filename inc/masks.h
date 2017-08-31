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

# define DISP_BIT 3 
# define DISP_MASK 0x08
# define HIDE 0x00
# define DISP 0x01

# define disp(mask) (mask | (1 << DISP_BIT))
# define hide(mask) (mask & ~(1 << DISP_BIT))
# define is_disp(mask) ((mask & DISP_MASK) != 0 ? 1 : 0)
# define is_hidden(mask) (!is_disp(mask)) 

# define DUMP_FORMAT_BIT 4 
# define DUMP_FORMAT_MASK 0x10

# define dump_4(mask) (mask | (1 << DUMP_FORMAT_BIT))
# define dump_1(mask) (mask & ~(1 << DUMP_FORMAT_BIT))

# define is_dump_4(mask) ((mask & DUMP_FORMAT_MASK) != 0 ? 1 : 0)
# define is_dump_1(mask) (!is_dump_4(mask))


# define nm(mask) (mask & NM_OTOOL_MASK)

#endif
