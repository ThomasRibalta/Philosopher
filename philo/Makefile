NAME = philo
CC = gcc
RM = rm -f
FLAGS = -Wall -Wextra -Werror

SRC_1 = brain.c \
	philosophe.c \
	utils.c \
	utils2.c \
			

OBJ_1 = ${SRC_1:.c=.o}

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJ_1}
	${CC} ${FLAGS} ${OBJ_1} main.c -o ${NAME}

all: ${NAME}

clean:
	${RM} ${OBJ_1} ${NAME}

fclean: clean
	${RM} ${NAME}
re: clean all

.PHONY: all clean fclean re
