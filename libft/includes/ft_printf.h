/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:17:38 by qliso             #+#    #+#             */
/*   Updated: 2024/11/27 14:17:43 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int		ft_print_char(char c);
int		ft_print_nbr(int nbr);
int		ft_print_hexnbr(unsigned int nbr, const char format);
int		ft_print_ptr(unsigned long long ptr);

int		ft_print_str(char *str);

int		ft_unbr_len(unsigned int nbr);
void	ft_put_unbr(unsigned int nbr);
int		ft_print_unbr(unsigned int nbr);

int		ft_print_format(va_list args, const char c);
int		ft_printf(const char *format, ...);

#endif
