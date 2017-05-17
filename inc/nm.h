/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/15 15:22:44 by ebouther          #+#    #+#             */
/*   Updated: 2017/05/15 17:37:10 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>

#                          include <stdlib.h> // remove norme
#                          include <string.h>

typedef struct 		s_sym
{
	uint64_t		value;
	char			type;
	char			*name;
	struct s_sym	*next;
}					t_sym;

#endif
