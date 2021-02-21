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

void	monitoring()
{
	int			i;
	uint64_t	current_time;
	uint64_t	time_last_eat;

	while (1)
	{
		usleep(100);
		i = 0;
		while (i < g_party->info->number_of_philosophers)
		{
			current_time = cur_time();
			time_last_eat = g_party->time_last_eat[i];
			if (current_time > time_last_eat &&
				(current_time - time_last_eat) > g_party->info->time_to_die
				&& g_party->is_anybody_die == -1)
			{
				pthread_mutex_destroy(g_party->print_mutex);
				g_party->is_anybody_die = i + 1;
				g_party->time_of_death = current_time - g_party->t0;
				return ;
			}
			if (g_party->end_of_sim == 1)
			{
				pthread_mutex_destroy(g_party->print_mutex);
				return ;
			}
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
		if ((ind.philo_ind % 2) == 0)
			pthread_mutex_lock(g_party->cutlery[ind.lfork_ind]);
		else
			pthread_mutex_lock(g_party->cutlery[ind.rfork_ind]);

		if (!(state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork", &ind)))
			return (NULL);

		if ((ind.philo_ind % 2) == 0)
			pthread_mutex_lock(g_party->cutlery[ind.rfork_ind]);
		else
			pthread_mutex_lock(g_party->cutlery[ind.lfork_ind]);

		if (!(state(cur_time() - g_party->t0, ind.philo_ind, "has taken a fork", &ind)))
			return (NULL);
		g_party->time_last_eat[ind.rfork_ind] = cur_time();
		if (!(state(cur_time() - g_party->t0, ind.philo_ind, "is eating", &ind)))
			return (NULL);
		precise_sleep(cur_time(), g_party->info->time_to_eat);
		pthread_mutex_unlock(g_party->cutlery[ind.lfork_ind]);
		pthread_mutex_unlock(g_party->cutlery[ind.rfork_ind]);
		if (i == g_party->info->number_of_times_each_philosopher_must_eat)
			break ;
		if (!(state(cur_time() - g_party->t0, ind.philo_ind, "is sleeping", &ind)))
			return (NULL);
		precise_sleep(cur_time(), g_party->info->time_to_sleep);
		if (!(state(cur_time() - g_party->t0, ind.philo_ind, "is thinking", &ind)))
			return (NULL);
	}
	g_party->end_of_sim = 1;
	return (NULL);
}
