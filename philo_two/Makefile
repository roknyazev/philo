
NAME			= philo_two

SRCS			= main.c ft_atoi.c utils.c sim.c init.c
OBJS			= $(SRCS:.c=.o)

CC			= gcc
RM			= rm -f
CFLAGS			= -Wall -Wextra -Werror


all:			$(NAME)

$(NAME):		$(OBJS)
				gcc ${CFLAGS} -o ${NAME} ${OBJS}

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re