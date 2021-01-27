
#ifndef PHILO_ONE_PHILO_ONE_H
# define PHILO_ONE_PHILO_ONE_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct		s_sim_info
{
	uint64_t 		number_of_philosophers;
	uint64_t 		time_to_die;
	uint64_t 		time_to_eat;
	uint64_t 		time_to_sleep;
	uint64_t 		number_of_times_each_philosopher_must_eat;
}					t_sim_info;

typedef struct		s_philo_party
{
	uint64_t		t0;
	t_sim_info		*info;
	int				is_anybody_die;
	uint64_t		*time_last_eat;
	uint64_t		time_of_death;
	pthread_mutex_t	**cutlery;
	pthread_mutex_t	*print_mutex;
}					t_philo_party;

typedef struct 		s_indexes
{
	int 			philo_ind;
	int 			rfork_ind;
	int 			lfork_ind;
}					t_indexes;

t_philo_party		*party;

int					ft_atoi(const char *nptr);

#endif
