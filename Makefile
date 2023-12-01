NAME = ircserv

CXX = clang++
INCLUDES = -I./include
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD $(INCLUDES)

SRC_DIR = ./src/
OBJ_DIR = ./objs/
SRC = Client.cpp \
	main.cpp \
	Server.cpp
OBJS = $(SRC:.cpp=.o)
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))
DEPS = $(OBJS:.o=.d)

# test:
#	@echo $(DEPS)

all: $(NAME)

debug: CXXFLAGS += -O0 -g3
debug: re

sanit: CXXFLAGS += -O0 -g3 -fsanitize=address
sanit: re

undefined: CXXFLAGS += -O0 -g -fsanitize=undefined
undefined: re

noerr: CXXFLAGS := $(filter-out -Werror,$(CXXFLAGS))
noerr: re

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all debug sanit undefined noerr clean fclean re
-include $(DEPS)
