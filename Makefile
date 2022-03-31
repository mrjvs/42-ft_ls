# ************************************************************************** #
#                                                                            #
#   Cats are the best!                                          _ |\_        #
#   And forced headers are annoying                             \` ..\       #
#                                                          __,.-" =__Y=      #
#   By: the cats                                         ."        )         #
#                                                  _    /   ,    \/\_        #
#   Created: // :: by                             ((____|    )_-\ \_-`       #
#   Updated: // :: by                             `-----'`-----` `--`        #
#                                                                            #
# ************************************************************************** #

NAME=ft_ls
CC=gcc
FLAGS=-Werror -Wall -Wextra

INCLUDES=./src/includes

HEADERS=\
	context.h
SOURCES=\
	main.c \
	context/context.c

SOURCES_PREFIXED=$(addprefix ./src/, $(SOURCES))
HEADERS_PREFIXED=$(addprefix ./src/includes/, $(HEADERS))
OBJ=$(SOURCES_PREFIXED:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -I $(INCLUDES) -o $@ $(OBJ)

%.o: %.c $(HEADERS_PREFIXED)
	$(CC) $(FLAGS) -I $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
