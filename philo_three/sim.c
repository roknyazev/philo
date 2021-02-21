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

#include "philo_three.h"

void	*monitoring(void)
{
	uint64_t	current_time;
	uint64_t	time_last_eat;

	while (1)
	{
		current_time = cur_time();
		time_last_eat = g_party->time_last_eat;
		usleep(100);
		if (current_time > time_last_eat &&
		(current_time - time_last_eat) > g_party->info->time_to_die
		&& cur_time() > current_time)
		{
			sem_post(g_party->print_sem);
			sem_close(g_party->print_sem);
			exit(g_party->is_anybody_die);
		}
	}
}

void	is_eating(t_indexes *ind)
{
	sem_wait(g_party->cutlery);
	if (!state(cur_time() - g_party->t0,
			ind->philo_ind, "has taken a fork", 1))
		exit(-1);
	sem_wait(g_party->cutlery);
	if (!state(cur_time() - g_party->t0,
			ind->philo_ind, "has taken a fork", 2))
		exit(-1);
	g_party->time_last_eat = cur_time();
	if (!state(cur_time() - g_party->t0, ind->philo_ind, "is eating", 2))
		exit(-1);
	precise_sleep(cur_time(), g_party->info->time_to_eat);
	sem_post(g_party->cutlery);
	sem_post(g_party->cutlery);
}

void	*lifecycle(void *philo_indexes)
{
	t_indexes	ind;
	uint64_t	i;
	pthread_t	monitor_thread;

	ind = *((t_indexes *)philo_indexes);
	g_party->is_anybody_die = ind.philo_ind;
	g_party->time_last_eat = cur_time();
	i = 0;
	pthread_create(&monitor_thread, NULL, monitoring, NULL);
	while (++i)
	{
		deadlock_protection(i, ind.philo_ind);
		is_eating(&ind);
		if (i == g_party->info->number_of_times_each_philosopher_must_eat)
			break ;
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "is sleeping", 0))
			exit(-1);
		precise_sleep(cur_time(), g_party->info->time_to_sleep);
		if (!state(cur_time() - g_party->t0, ind.philo_ind, "is thinking", 0))
			exit(-1);
	}
	exit(0);
}
