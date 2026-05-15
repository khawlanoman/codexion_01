NAME = codexion

RM = rm -f
CC = CC
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread

SRC = create_coders.c \
	  create_dongles.c \
	  f_h_lock_dongles.c \
	  f_h_thread_f.c \
	  f_h2_thread_f.c \
	  heap.c \
	  main.c \
	  parsing.c \
	  timer.c 

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
