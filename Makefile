NAME = philo

CC = cc 
CFLAGS = -Wall -Wextra -Werror


SRCS = 	input_parse.c \
        philo.c \
		philo_utils.c \
		init.c \
		dead.c \
		utils_2.c

OBJS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

clean: 
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re