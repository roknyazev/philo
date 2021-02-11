/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:28:20 by wrudy             #+#    #+#             */
/*   Updated: 2021/02/06 20:28:22 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	monitoring(void)
{
	int			i;
	uint64_t	current_time;
	uint64_t	time_last_eat;

	while (1)
	{
		i = 0;
		while (i < g_party->info->number_of_philosophers)
		{
			current_time = cur_time();
			time_last_eat = g_party->time_last_eat[i];
			if (current_time > time_last_eat &&
				(current_time - time_last_eat) > g_party->info->time_to_die
				&& g_party->is_anybody_die == -1)
			{
				g_party->is_anybody_die = i + 1;
				g_party->time_of_death = current_time - g_party->t0;
				sem_post(g_party->print_sem);
				sem_close(g_party->print_sem);
				return ;
			}
			if (g_party->end_of_sim == 1)
				return ;
			i++;
		}
	}
}

void	*lifecycle(void *philo_indexes)
{
	t_indexes	ind;
	uint64_t	i;

	ind = *((t_indexes *)philo_indexes);
	i = 0;
	while (++i)
	{
		deadlock_protection(i, ind.philo_ind);
		sem_wait(g_party->cutlery);
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork", 1))
			break;
		sem_wait(g_party->cutlery);
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork", 2))
			break;
		g_party->time_last_eat[ind.rfork_ind] = cur_time();
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "is eating", 2))
			break;
		precise_sleep(cur_time(), g_party->info->time_to_eat);
		sem_post(g_party->cutlery);
		sem_post(g_party->cutlery);
		if (i == g_party->info->number_of_times_each_philosopher_must_eat)
			break ;
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "is sleeping", 0))
			break;
		precise_sleep(cur_time(), g_party->info->time_to_sleep);
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "is thinking", 0))
			break;
	}
	g_party->end_of_sim = 1;
	return (NULL);
}
