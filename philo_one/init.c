/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:27:42 by wrudy             #+#    #+#             */
/*   Updated: 2021/02/06 20:27:48 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	secure_exit(pthread_t *threads, t_indexes *indexes)
{
	int i;

	if (threads != NULL)
		free(threads);
	if (indexes != NULL)
		free(indexes);
	if (g_party->time_last_eat != NULL)
		free(g_party->time_last_eat);
	if (g_party->print_mutex != NULL)
		free(g_party->print_mutex);
	i = 0;
	if (g_party->cutlery != NULL)
	{
		while (i < g_party->info->number_of_philosophers
		&& (g_party->cutlery)[i] != NULL)
		{
			pthread_mutex_destroy((g_party->cutlery)[i]);
			free((g_party->cutlery)[i]);
			i++;
		}
		free(g_party->cutlery);
	}
	free(g_party);
}

int		init_threads(pthread_t **threads)
{
	if (!(*threads = malloc(sizeof(pthread_t) *
							g_party->info->number_of_philosophers)))
		return (0);
	return (1);
}

int		init_indexes(t_indexes **indexes)
{
	int i;

	if (!(*indexes = malloc(sizeof(t_indexes) *
							g_party->info->number_of_philosophers)))
		return (0);
	i = 0;
	while (i < (g_party->info->number_of_philosophers - 1))
	{
		(*indexes)[i].philo_ind = i + 1;
		(*indexes)[i].rfork_ind = i;
		(*indexes)[i].lfork_ind = i + 1;
		i++;
	}
	(*indexes)[i].philo_ind = i + 1;
	(*indexes)[i].rfork_ind = i;
	(*indexes)[i].lfork_ind = 0;
	return (1);
}

int		init_party(void)
{
	int i;

	if (!(g_party->time_last_eat = malloc(sizeof(uint64_t) *
									g_party->info->number_of_philosophers)))
		return (0);
	if (!(g_party->print_mutex = malloc(sizeof(pthread_mutex_t))))
		return (0);
	if (!(g_party->cutlery = malloc(sizeof(pthread_mutex_t *) *
									g_party->info->number_of_philosophers)))
		return (0);
	i = 0;
	while (i < g_party->info->number_of_philosophers)
	{
		if (!((g_party->cutlery)[i] = malloc(sizeof(pthread_mutex_t))))
		{
			g_party->cutlery[i] = NULL;
			return (0);
		}
		i++;
	}
	while (i--)
		pthread_mutex_init((g_party->cutlery)[i], NULL);
	pthread_mutex_init(g_party->print_mutex, NULL);
	return (1);
}

int		init(pthread_t **threads, t_indexes **indexes)
{
	*threads = NULL;
	*indexes = NULL;
	g_party->time_last_eat = NULL;
	g_party->print_mutex = NULL;
	g_party->cutlery = NULL;
	g_party->is_anybody_die = -1;
	g_party->end_of_sim = 0;
	if (!init_indexes(indexes) || !init_threads(threads) || !init_party())
	{
		secure_exit(*threads, *indexes);
		return (0);
	}
	return (1);
}
