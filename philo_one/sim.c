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

#include "philo_one.h"

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
				return ;
			}
			if (g_party->end_of_sim == 1)
				return ;
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
		pthread_mutex_lock(g_party->cutlery[ind.rfork_ind]);
		state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork");
		pthread_mutex_lock(g_party->cutlery[ind.lfork_ind]);
		state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork");
		g_party->time_last_eat[ind.rfork_ind] = cur_time();
		state(cur_time() - g_party->t0, ind.philo_ind, "is eating");
		precise_sleep(cur_time(), g_party->info->time_to_eat);
		pthread_mutex_unlock(g_party->cutlery[ind.lfork_ind]);
		pthread_mutex_unlock(g_party->cutlery[ind.rfork_ind]);
		if (i == g_party->info->number_of_times_each_philosopher_must_eat)
			break ;
		state(cur_time() - g_party->t0, ind.philo_ind, "is sleeping");
		precise_sleep(cur_time(), g_party->info->time_to_sleep);
		state(cur_time() - g_party->t0, ind.philo_ind, "is thinking");
	}
	g_party->end_of_sim = 1;
	return (NULL);
}
