/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouther <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 20:28:44 by ebouther          #+#    #+#             */
/*   Updated: 2017/09/08 12:00:09 by ebouther         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "masks.h"

void	disp_err(char *bin, char *f, char *err);
void	hexdump(void *data, size_t size, uint64_t addr, uint8_t arch_64);

#endif
