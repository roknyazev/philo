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

#include "philo_two.h"

uint64_t	cur_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int			state(uint64_t timestamp, int index, char *msg, int count)
{
	if (sem_wait(g_party->print_sem))
	{
		while (count)
		{
			sem_post(g_party->cutlery);
			count--;
		}
		return (0);
	}
	printf("%llu: philosopher %d %s\n", timestamp, index, msg);
	sem_post(g_party->print_sem);
	return (1);
}

void		precise_sleep(uint64_t start_time, uint64_t ms_sleep)
{
	while ((cur_time() - start_time) < ms_sleep)
	{
		if (g_party->is_anybody_die > 0)
			return ;
		usleep(100);
	}
}

void		deadlock_protection(uint64_t i, int philo_index)
{
	if (i == 1 && (philo_index % 2) == 0)
		usleep(100);
}

int			error(char *text, int code)
{
	printf("%s\n", text);
	return (code);
}
