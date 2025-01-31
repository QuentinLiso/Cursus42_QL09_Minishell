/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:19:52 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:19:55 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_unbr(unsigned int nb)
{
	int	len;

	len = 0;
	if (nb >= 10)
	{
		len += ft_print_unbr(nb / 10);
		len += ft_print_unbr(nb % 10);
	}
	else
		len += ft_print_char(nb + '0');
	if (len < 0)
		return (-1);
	return (len);
}
