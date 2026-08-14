NAME = codexion

RM = rm -f
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

SRC = codexion_src/create_coders.c \
	  codexion_src/create_dongles.c \
	  codexion_src/f_h_lock_dongles.c \
	  codexion_src/f_h_thread_f.c \
	  codexion_src/f_h2_thread_f.c \
	  codexion_src/heap.c \
	  codexion_src/main.c \
	  codexion_src/parsing.c \
	  codexion_src/timer.c \
	  codexion_src/timer_helper1.c \
	  codexion_src/timer_helper2.c \
	  codexion_src/main_helper.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
