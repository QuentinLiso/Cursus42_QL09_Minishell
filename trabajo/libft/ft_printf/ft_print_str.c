/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:20:22 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:20:28 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_str(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (write (1, "(null)", 6));
	while (str[len])
		write (1, &str[len++], 1);
	return (len);
}
