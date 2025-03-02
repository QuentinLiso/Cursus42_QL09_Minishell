/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hexnbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:20:04 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:20:07 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_hexnbr(unsigned int nbr, const char format)
{
	char	*hex;
	int		len;

	if (format == 'x')
		hex = "0123456789abcdef";
	else
		hex = "0123456789ABCDEF";
	len = 0;
	if (nbr >= 16)
	{
		len += ft_print_hexnbr(nbr / 16, format);
		len += ft_print_hexnbr(nbr % 16, format);
	}
	else
		len += ft_print_char(hex[nbr % 16]);
	if (len < 0)
		return (-1);
	return (len);
}
