/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <wrudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/20 14:17:41 by wrudy             #+#    #+#             */
/*   Updated: 2020/05/29 15:36:37 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int			ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	get_nmb(const char *str)
{
	size_t number;
	size_t order;
	size_t len;

	order = 1;
	len = 0;
	while (ft_isdigit(str[len]))
		len++;
	number = (int)(*str) - '0';
	while (order < len)
	{
		number = number * 10;
		order++;
	}
	if (order > 1)
		return (number + get_nmb(str + 1));
	else
		return (number);
}

int			ft_atoi(const char *nptr)
{
	int sign;

	while (*nptr == ' ' || *nptr == '\v' || *nptr == '\r' \
		|| *nptr == '\t' || *nptr == '\n' || *nptr == '\f')
		nptr++;
	if (*nptr == '-')
		sign = -1;
	else
		sign = 1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	if (ft_isdigit(*nptr))
		return (get_nmb(nptr) * sign);
	else
		return (0);
}
