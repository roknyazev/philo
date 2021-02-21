cmake_minimum_required(VERSION 3.17)
project(philo_three C)

set(CMAKE_C_STANDARD 99)

add_executable(philo_one main.c philo_three.h ft_atoi.c utils.c sim.c init.c)