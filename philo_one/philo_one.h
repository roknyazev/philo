
#ifndef PHILO_ONE_PHILO_ONE_H
# define PHILO_ONE_PHILO_ONE_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct		s_sim_info
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
}					t_sim_info;

typedef struct		s_philo_party
{
	t_sim_info		*info;
	int				is_anybody_die;
	uint64_t		current_time;
	uint64_t		time_of_death;
	pthread_mutex_t	**cutlery;
	pthread_mutex_t	*print_mutex;

}					t_philo_party;

typedef struct 		s_indexes
{
	int 			philo_index;
	int 			right_fork_index;
	int 			left_fork_index;
}					t_indexes;

t_philo_party		*fan_meeting;

int					ft_atoi(const char *nptr);

#endif
