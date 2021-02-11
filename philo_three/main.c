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

#include "philo_two.h"

int	simulate(void)
{
	pthread_t		*threads;
	t_indexes		*indexes;
	int				i;
	pid_t			process_id;
	int 			status;

	if (!init(&threads, &indexes))
		return (error("Init error", EXIT_FAILURE));
	i = 0;
	while (i < g_party->info->number_of_philosophers)
	{
		process_id = fork();
		if (process_id == 0)
		{
			lifecycle(indexes);
		}
		i++;
	}
	waitpid(-1, &status, 0);

	sem_close(g_party->cutlery);
	if (WEXITSTATUS(status) > 0)
		printf("%llu: philosopher %d %s\n",
				cur_time(), WEXITSTATUS(status), "died");
	printf("End of simulation\n");
	secure_exit(threads, indexes);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_sim_info sim_info;

	if (argc != 5 && argc != 6)
		return (error("Wrong number of arguments", 1));
	if (((sim_info.number_of_philosophers = ft_atoi(argv[1])) < 1))
		return (error("Invalid number of philosophers", 1));
	if (((sim_info.time_to_die = ft_atoi(argv[2])) < 1))
		return (error("Invalid time to die", 1));
	if (((sim_info.time_to_eat = ft_atoi(argv[3])) < 1))
		return (error("Invalid time to eat", 1));
	if (((sim_info.time_to_sleep = ft_atoi(argv[4])) < 1))
		return (error("Invalid time to sleep", 1));
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
