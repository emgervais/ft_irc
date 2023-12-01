NAME = ircserv

CXX = clang++
INCLUDES = -I./include
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD $(INCLUDES)

SRC_DIR = ./src/
OBJ_DIR = ./objs/
SRC = Client.cpp \
	main.cpp \
<<<<<<< HEAD
	Server.cpp 
=======
	Server.cpp \
	ServerCmdHandler.cpp
>>>>>>> 4f4b308d84929d9f579d45e0baa0ff06c409b2f2
OBJS = $(SRC:.cpp=.o)
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))
DEPS = $(OBJS:.o=.d)

all: $(NAME)

debug: CXXFLAGS += -O0 -g3
debug: re

sanit: CXXFLAGS += -O0 -g3 -fsanitize=address
sanit: re

undefined: CXXFLAGS += -O0 -g3 -fsanitize=undefined
undefined: re

noerr: CXXFLAGS := $(filter-out -Werror,$(CXXFLAGS))
noerr: re

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all debug sanit undefined noerr clean fclean re
-include $(DEPS)
