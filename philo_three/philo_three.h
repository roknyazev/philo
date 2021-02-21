/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wrudy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 20:28:10 by wrudy             #+#    #+#             */
/*   Updated: 2021/02/06 20:28:12 by wrudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_PHILO_ONE_H
# define PHILO_ONE_PHILO_ONE_H
# define SEMAPHORE_NAME1 "/philo_semaphore1"
# define SEMAPHORE_NAME2 "/philo_semaphore2"
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <unistd.h>
# include <sys/time.h>
# include <fcntl.h>
# include <signal.h>

typedef struct		s_sim_info
{
	uint64_t		number_of_philosophers;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		number_of_times_each_philosopher_must_eat;
}					t_sim_info;

typedef struct		s_philo_party
{
	t_sim_info		*info;
	int				is_anybody_die;
	uint64_t		t0;
	uint64_t		time_last_eat;
	sem_t			*print_sem;
	sem_t			*cutlery;
}					t_philo_party;

typedef struct		s_indexes
{
	int				philo_ind;
	int				rfork_ind;
}					t_indexes;

t_philo_party		*g_party;
int					ft_atoi(const char *nptr);
uint64_t			cur_time(void);
int					state(uint64_t timestamp, int index, char *msg, int count);
void				precise_sleep(uint64_t start_time, uint64_t ms_sleep);
void				deadlock_protection(uint64_t i, int philo_index);
void				*monitoring();
void				*lifecycle(void *philo_indexes);
int					init(pthread_t **threads, t_indexes **indexes);
void				secure_exit(pthread_t *threads, t_indexes *indexes);
int					error(char *text, int code);

#endif
