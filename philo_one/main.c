
#include "philo_one.h"

uint64_t cur_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_state(uint64_t timestamp, int index, char *msg)
{
	pthread_mutex_lock(party->print_mutex);
	printf("%llu: philosopher %d %s\n", timestamp, index, msg);
	pthread_mutex_unlock(party->print_mutex);
}

void precise_sleep(uint64_t start_time, uint64_t ms_sleep)
{
	while ((cur_time() - start_time) < ms_sleep)
		usleep(100);
}

void *trace()
{
	int i;
	uint64_t current_time;
	uint64_t time_last_eat;

	while (1)
	{
		i = 0;
		while (i < party->info->number_of_philosophers)
		{
			current_time = cur_time();
			time_last_eat = party->time_last_eat[i];
			if (current_time > time_last_eat &&
			(current_time - time_last_eat) > party->info->time_to_die
			&& party->is_anybody_die == -1)
			{
				party->is_anybody_die = i + 1;
				party->time_of_death = cur_time() - party->t0;
				return (NULL);
			}
		}
	}
}

void deadlock_protection(uint64_t i, int philo_index)
{
	if (i == 1 && (philo_index % 2) == 0)
		usleep(100);
}

void *lifecycle(void *philo_indexes)
{
	t_indexes	ind;
	uint64_t	i;

	ind = *((t_indexes *)philo_indexes);
	i = 0;
	while (++i)
	{
		deadlock_protection(i, ind.philo_ind);
		pthread_mutex_lock(party->cutlery[ind.rfork_ind]);
		print_state(cur_time() - party->t0, ind.philo_ind, "has taken a fork");
		pthread_mutex_lock(party->cutlery[ind.lfork_ind]);
		print_state(cur_time() - party->t0, ind.philo_ind, "has taken a fork");
		party->time_last_eat[ind.rfork_ind] = cur_time();
		print_state(cur_time() - party->t0, ind.philo_ind, "is eating");
		precise_sleep(cur_time(), party->info->time_to_eat);
		pthread_mutex_unlock(party->cutlery[ind.lfork_ind]);
		pthread_mutex_unlock(party->cutlery[ind.rfork_ind]);
		if (i == party->info->number_of_times_each_philosopher_must_eat)
			break;
		print_state(cur_time() - party->t0, ind.philo_ind, "is sleeping");
		precise_sleep(cur_time(), party->info->time_to_sleep);
		print_state(cur_time() - party->t0, ind.philo_ind, "is thinking");
	}
	party->is_anybody_die--;
	return (NULL);
}

int simulate(t_sim_info *info)
{
	pthread_t		*threads;
	pthread_t 		trace_thread;
	t_indexes		*indexes;
	int				i;

	if (!(threads = malloc(sizeof(pthread_t) * info->number_of_philosophers)))
		return (EXIT_FAILURE);
	if (!(indexes = malloc(sizeof(t_indexes) * info->number_of_philosophers)))
	{
		free(threads);
		return (EXIT_FAILURE);
	}
	if (!(party->time_last_eat = malloc(sizeof(uint64_t) * info->number_of_philosophers)))
	{
		free(indexes);
		free(threads);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < (info->number_of_philosophers - 1))
	{
		indexes[i].philo_ind = i + 1;
		indexes[i].rfork_ind = i;
		indexes[i].lfork_ind = i + 1;
		i++;
	}
	indexes[i].philo_ind = i + 1;
	indexes[i].rfork_ind = i;
	indexes[i].lfork_ind = 0;
	if (!(party->cutlery  = malloc(sizeof(pthread_mutex_t *) * info->number_of_philosophers)))
	{
		free(party->time_last_eat);
		free(indexes);
		free(threads);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < info->number_of_philosophers)
	{
		(party->cutlery)[i] = malloc(sizeof(pthread_mutex_t));
		i++;
	}
	party->print_mutex = malloc(sizeof(pthread_mutex_t));
	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_init((party->cutlery)[i], NULL);
		i++;
	}
	pthread_mutex_init(party->print_mutex, NULL);
	i = 0;
	party->t0 = cur_time();
	while (i < info->number_of_philosophers)
	{
		party->time_last_eat[i] = party->t0;
		pthread_create(&threads[i], NULL, lifecycle, &indexes[i]);
		i++;
	}
	pthread_create(&trace_thread, NULL, trace, NULL);
	i = 0;
	while (1)
	{
		if (party->is_anybody_die >= 0)
		{
			pthread_detach(trace_thread);
			while (i < info->number_of_philosophers)
			{
				pthread_detach(threads[i]);
				i++;
			}
			printf("%llu: philosopher %d %s\n", party->time_of_death, party->is_anybody_die, "died");
			break;
		}
		if (party->is_anybody_die == -party->info->number_of_philosophers - 1)
		{
			pthread_detach(trace_thread);
			while (i < info->number_of_philosophers)
			{
				pthread_detach(threads[i]);
				i++;
			}
			break;
		}
	}
	printf("End of simulation");
	free(indexes);
	free(threads);
	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_destroy(party->cutlery[i]);
		i++;
	}
	pthread_mutex_destroy(party->print_mutex);
	return (0);
}

int main(int argc, char *argv[])
{
	t_sim_info sim_info;

	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments\n");
		return (1);
	}

	sim_info.number_of_philosophers = ft_atoi(argv[1]);
	sim_info.time_to_die = ft_atoi(argv[2]);
	sim_info.time_to_eat = ft_atoi(argv[3]);
	sim_info.time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		sim_info.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		sim_info.number_of_times_each_philosopher_must_eat = INT64_MAX;
	if (!(party = malloc(sizeof(t_philo_party))))
		return (EXIT_FAILURE);
	party->info = &sim_info;
	party->is_anybody_die = -1;

	return (simulate(&sim_info));
}
