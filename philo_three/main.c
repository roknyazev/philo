/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:27:56 by wrudy             #+#    #+#             */
/*   Updated: 2021/02/06 20:27:59 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		is_numbers(int argc, char **argv)
{
	int i;
	int j;

	j = 1;
	while (j < argc)
	{
		i = 0;
		while (argv[j][i] != '\0')
		{
			if (argv[j][i] < '0' || argv[j][i] > '9')
				return (0);
			i++;
		}
		j++;
	}
	return (1);
}

void	finish(int i, pthread_t *threads, t_indexes *indexes)
{
	int			status;

	waitpid(-1, &status, 0);
	sem_close(g_party->cutlery);
	if (WEXITSTATUS(status) > 0)
		printf("%llu: philosopher %d %s\n",
			cur_time() - g_party->t0, WEXITSTATUS(status), "died");
	else
	{
		while (i--)
			waitpid(-1, &status, 0);
	}
	printf("End of simulation\n");
	secure_exit(threads, indexes);
	kill(0, SIGINT);
}

int		simulate(void)
{
	pthread_t		*threads;
	t_indexes		*indexes;
	int				i;
	pid_t			process_id;

	if (!init(&threads, &indexes))
		return (error("Init error", EXIT_FAILURE));
	i = 0;
	g_party->t0 = cur_time();
	while (i < g_party->info->number_of_philosophers)
	{
		process_id = fork();
		if (process_id == 0)
			lifecycle(&(indexes[i]));
		i++;
	}
	finish(i, threads, indexes);
	return (0);
}

int		main(int argc, char *argv[])
{
	t_sim_info sim_info;

	if ((argc != 5 && argc != 6) || !is_numbers(argc, argv))
		return (error("Wrong arguments", 1));
	if (((sim_info.number_of_philosophers = ft_atoi(argv[1])) <= 1))
		return (error("Invalid number of philosophers", 1));
	if (((sim_info.time_to_die = ft_atoi(argv[2])) < 40))
		return (error("Invalid time to die", 1));
	if (((sim_info.time_to_eat = ft_atoi(argv[3])) < 20))
		return (error("Invalid time to eat", 10));
	if (((sim_info.time_to_sleep = ft_atoi(argv[4])) < 1))
		return (error("Invalid time to sleep", 20));
	if (argv[5])
	{
		if (ft_atoi(argv[5]) < 1)
			return (error("Invalid number of times each philo must eat", 1));
		sim_info.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	}
	else
		sim_info.number_of_times_each_philosopher_must_eat = INT64_MAX;
	if (!(g_party = malloc(sizeof(t_philo_party))))
		return (error("Memory error", EXIT_FAILURE));
	g_party->info = &sim_info;
	return (simulate());
}
