#include "nm_otool.h"

void handle_ar(char *f, char *ptr)
{
	struct ar_hdr	*ar_header;	
	unsigned int 	filename_pad;
	char 			*filename;

	ar_header = (struct ar_hdr *)(ptr + SARMAG);
	int ar_size = ft_atoi(ar_header->ar_size);

	while ((ar_header = (struct ar_hdr *)((void*)ar_header + sizeof(struct ar_hdr) + ar_size))
		   && ((char *)ar_header)[0]) {

		
		ar_size = ft_atoi(ar_header->ar_size);
		printf("SIZE : (%d)\n", ar_size);

		if (ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) == 0)
		{
			filename_pad = ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1));
			filename = ar_header->ar_name + sizeof(struct ar_hdr);
			ft_printf("\n%s(%s):\n", f, filename);
			handlers(NULL, 0, (char *)ar_header + sizeof(struct ar_hdr) + filename_pad, 0);
		} else {
		
			filename = ar_header->ar_name;
			int i = 0;
			while (filename[i]) {
				if (filename[i] == 0x20)
				{
					filename[i] = 0;
					break ;
				}
				i++;
			}
			ft_printf("\n%s(%s):\n", f, filename);
			handlers(NULL, 0, (char *)ar_header + sizeof(struct ar_hdr), 0);
		}
	}
}
