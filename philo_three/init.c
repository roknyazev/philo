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

#include "philo_three.h"

void	secure_exit(pthread_t *threads, t_indexes *indexes)
{
	if (indexes != NULL)
		free(indexes);
	if (threads != NULL)
		free(threads);
	sem_unlink(SEMAPHORE_NAME1);
	sem_unlink(SEMAPHORE_NAME2);
	free(g_party);
}

int		init_threads(pthread_t **threads)
{
	if (!(*threads = malloc(sizeof(pthread_t) *
							g_party->info->number_of_philosophers)))
		return (error("malloc error", 0));
	return (1);
}

int		init_indexes(t_indexes **indexes)
{
	int i;

	if (!(*indexes = malloc(sizeof(t_indexes) *
							g_party->info->number_of_philosophers)))
		return (error("malloc error", 0));
	i = 0;
	while (i < (g_party->info->number_of_philosophers - 1))
	{
		(*indexes)[i].philo_ind = i + 1;
		(*indexes)[i].rfork_ind = i;
		i++;
	}
	(*indexes)[i].philo_ind = i + 1;
	(*indexes)[i].rfork_ind = i;
	return (1);
}

int		init_party(void)
{
	sem_unlink(SEMAPHORE_NAME1);
	sem_unlink(SEMAPHORE_NAME2);
	if ((g_party->print_sem = sem_open(SEMAPHORE_NAME1, O_CREAT, 0777, 1)) == SEM_FAILED)
		return (error("sem_open error", 0));
	if ((g_party->cutlery = sem_open(SEMAPHORE_NAME2, O_CREAT, 0777,
							g_party->info->number_of_philosophers)) == SEM_FAILED)
		return (error("sem_open error", 0));
	return (1);
}

int		init(pthread_t **threads, t_indexes **indexes)
{
	*threads = NULL;
	*indexes = NULL;
	g_party->print_sem = NULL;
	g_party->cutlery = NULL;
	if (!init_indexes(indexes) || !init_threads(threads) || !init_party())
	{
		secure_exit(*threads, *indexes);
		return (0);
	}
	return (1);
}
