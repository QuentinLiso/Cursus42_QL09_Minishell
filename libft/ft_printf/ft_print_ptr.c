/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:20:43 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:20:46 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_hexnbrull(unsigned long long nbr, const char format)
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
		len += ft_print_hexnbrull(nbr / 16, format);
		len += ft_print_hexnbrull(nbr % 16, format);
	}
	else
		len += ft_print_char(hex[nbr % 16]);
	if (len < 0)
		return (-1);
	return (len);
}

int	ft_print_ptr(unsigned long long ptr)
{
	int		len;

	if (ptr == 0)
		return (ft_print_str("(nil)"));
	len = 0;
	len += ft_print_str("0x");
	len += ft_print_hexnbrull(ptr, 'x');
	if (len < 0)
		return (-1);
	return (len);
}
