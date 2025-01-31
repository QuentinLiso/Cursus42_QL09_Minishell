/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:18:23 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:18:34 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_format(va_list args, const char c)
{
	int		len;

	len = 0;
	if (c == 'c')
		len += ft_print_char(va_arg(args, int));
	else if (c == 's')
		len += ft_print_str(va_arg(args, char *));
	else if (c == 'p')
		len += ft_print_ptr(va_arg(args, unsigned long long));
	else if (c == 'd' || c == 'i')
		len += ft_print_nbr(va_arg(args, int));
	else if (c == 'u')
		len += ft_print_unbr(va_arg(args, unsigned int));
	else if (c == 'x')
		len += ft_print_hexnbr(va_arg(args, int), c);
	else if (c == 'X')
		len += ft_print_hexnbr(va_arg(args, int), c);
	else if (c == '%')
		len += ft_print_char('%');
	return (len);
}

// Fonction printf
int	ft_printf(const char *format, ...)
{
	va_list			args;
	int				i;
	int				len;

	if (!format)
		return (-1);
	i = 0;
	len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			len += ft_print_format(args, format[i + 1]);
			i++;
		}
		else
			len += ft_print_char(format[i]);
		i++;
	}
	va_end(args);
	return (len);
}
