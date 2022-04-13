NAME=ft_ls
CC=gcc
FLAGS=-Werror -Wall -Wextra -g

INCLUDES=./src/includes

HEADERS=\
	context.h \
	bool.h \
	cli.h \
	io.h \
	list.h

SOURCES=\
	main.c \
	context/context.c \
	cli/args.c \
	cli/parse.c \
	io/file.c \
	io/gather.c \
	io/directory.c \
	utils/list.c

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