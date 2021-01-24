
#include "philo_one.h"

uint64_t get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_state(uint64_t timestamp, int index, char *msg, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	printf("%llu: philosopher %d %s\n", timestamp, index, msg);
	pthread_mutex_unlock(mutex);
}

void *lifecycle(void *philo_indexes)
{
	t_indexes indexes;
	uint64_t time_of_start_eating;
	uint64_t t0;
	pthread_mutex_t *mutex1;
	pthread_mutex_t *mutex2;
	uint64_t ttd = fan_meeting->info->time_to_die;
	uint64_t tte = fan_meeting->info->time_to_eat * 1000;
	uint64_t tts = fan_meeting->info->time_to_sleep * 1000;


	indexes = *((t_indexes *)philo_indexes);
	mutex1 = fan_meeting->cutlery[indexes.right_fork_index];
	mutex2 = fan_meeting->cutlery[indexes.left_fork_index];


	t0 = get_time();
	time_of_start_eating = t0;
	while (1)
	{
		//printf("%llu\n", get_time() - time_of_start_eating);

		pthread_mutex_lock(mutex1);

		print_state(get_time() - t0, indexes.philo_index, "has taken a fork", fan_meeting->print_mutex);
		pthread_mutex_lock(mutex2);
		print_state(get_time() - t0, indexes.philo_index, "has taken a fork", fan_meeting->print_mutex);

		if ((get_time() - time_of_start_eating)  >= ttd)
		{
			fan_meeting->is_anybody_die = indexes.philo_index;
			fan_meeting->time_of_death = get_time() - t0;
			break;
		}

		print_state(get_time() - t0, indexes.philo_index, "is eating", fan_meeting->print_mutex);
		time_of_start_eating = get_time();
		usleep(tte);


		pthread_mutex_unlock(mutex2);
		pthread_mutex_unlock(mutex1);

		print_state(get_time() - t0, indexes.philo_index, "is sleeping", fan_meeting->print_mutex);

		usleep(tts);

		print_state(get_time() - t0, indexes.philo_index, "is thinking", fan_meeting->print_mutex);
	}
	return (NULL);
}

void simulate(t_sim_info *info)
{
	pthread_t		*threads;
	pthread_mutex_t	**cutlery;
	t_indexes		*indexes;
	int				i;

	if (!(threads = malloc(sizeof(pthread_t) * info->number_of_philosophers)))
		return;
	if (!(indexes = malloc(sizeof(t_indexes) * info->number_of_philosophers)))
	{
		free(threads);
		return;
	}
	i = 0;
	while (i < (info->number_of_philosophers - 1))
	{
		indexes[i].philo_index = i + 1;
		indexes[i].right_fork_index = i;
		indexes[i].left_fork_index = i + 1;
		i++;
	}
	indexes[i].philo_index = i + 1;
	indexes[i].right_fork_index = i;
	indexes[i].left_fork_index = 0;
	if (!(cutlery  = malloc(sizeof(pthread_mutex_t *) * info->number_of_philosophers)))
	{
		free(indexes);
		free(threads);
		return;
	}
	i = 0;
	while (i < info->number_of_philosophers)
	{
		cutlery[i] = malloc(sizeof(pthread_mutex_t));
		i++;
	}
	fan_meeting->print_mutex = malloc(sizeof(pthread_mutex_t));
	fan_meeting->cutlery = cutlery;
	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_init(cutlery[i], NULL);
		i++;
	}
	pthread_mutex_init(fan_meeting->print_mutex, NULL);
	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_create(&threads[i], NULL, lifecycle, &indexes[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (1)
	{
		if (fan_meeting->is_anybody_die >= 0)
		{
			while (i < info->number_of_philosophers)
			{
				pthread_detach(threads[i]);
				i++;
			}
			printf("%llu: philosopher %d %s\n", fan_meeting->time_of_death, fan_meeting->is_anybody_die, "died");
			break;
		}
	}
	free(indexes);
	free(threads);
	i = 0;
	while (i < info->number_of_philosophers)
	{
		pthread_mutex_destroy(cutlery[i]);
		i++;
	}
	pthread_mutex_destroy(fan_meeting->print_mutex);
	printf("\n\nFINISH");
}

int main(int argc, char *argv[])
{
	t_sim_info sim_info;

	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments");
		return (1);
	}

	sim_info.number_of_philosophers = ft_atoi(argv[1]);
	sim_info.time_to_die = ft_atoi(argv[2]);
	sim_info.time_to_eat = ft_atoi(argv[3]);
	sim_info.time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		sim_info.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);

	if (!(fan_meeting = malloc(sizeof(t_philo_party))))
		return (EXIT_FAILURE);
	fan_meeting->info = &sim_info;
	fan_meeting->is_anybody_die = -1;

	simulate(&sim_info);
	return (0);
}
