/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:28:29 by wrudy             #+#    #+#             */
/*   Updated: 2021/02/06 20:28:32 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t	cur_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int			state(uint64_t timestamp, char *msg, t_indexes *ind)
{
	if (pthread_mutex_lock(g_party->print_mutex) != 0)
	{
		pthread_mutex_unlock(g_party->cutlery[ind->rfork_ind]);
		pthread_mutex_unlock(g_party->cutlery[ind->lfork_ind]);
		return (0);
	}
	printf("%llu: philosopher %d %s\n", timestamp, ind->philo_ind, msg);
	pthread_mutex_unlock(g_party->print_mutex);
	return (1);
}

void		precise_sleep(uint64_t start_time, uint64_t ms_sleep)
{
	while ((cur_time() - start_time) < ms_sleep)
		usleep(100);
}

int			error(char *text, int code)
{
	printf("%s\n", text);
	return (code);
}
