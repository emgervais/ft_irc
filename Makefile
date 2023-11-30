NAME = ircserv

CC = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address
INCLUDES = -I./include

SRCS_DIR = ./src/
SRCS_FILES = main.cpp Server.cpp Client.cpp
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS_DIR = ./objs/
OBJS_FILES = $(SRCS_FILES:.cpp=.o)
OBJS = $(addprefix $(OBJS_DIR), $(OBJS_FILES))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
