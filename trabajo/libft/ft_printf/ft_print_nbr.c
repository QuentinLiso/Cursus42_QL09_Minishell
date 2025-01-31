/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:21:14 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:21:17 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_nbr(int nb)
{
	int	len;

	len = 0;
	if (nb == -2147483648)
		len += write(1, "-2147483648", 11);
	else if (nb < 0)
	{
		len += ft_print_char('-');
		len += ft_print_nbr(-nb);
	}
	else if (nb > 9)
	{
		len += ft_print_nbr(nb / 10);
		len += ft_print_nbr(nb % 10);
	}
	else
		len += ft_print_char(nb + '0');
	if (len < 0)
		return (-1);
	return (len);
}
