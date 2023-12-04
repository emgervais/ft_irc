NAME = ircserv

INCLUDES = -I./include
SRC_DIR = src/
OBJ_DIR = obj/

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD $(INCLUDES)

SRC = Server/Server.cpp Server/UnixSignals.cpp Server/ServerLoop.cpp \
	Client/Client.cpp Client/ClientRegistration.cpp \
	Commands/Command.cpp Commands/Registration.cpp Commands/Connection.cpp \
	main.cpp

SRC := $(addprefix $(SRC_DIR), $(SRC))
OBJ := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
DEP := $(OBJ:%.o=%.d)

GREEN = "\033[32m
YELLOW = "\033[33m
RED = "\033[31m
NO_COLOR = \033[0m"

all: $(NAME)

debug: CXXFLAGS += -O0 -g3
debug: re

sanit: CXXFLAGS += -O0 -g3 -fsanitize=address
sanit: re

undefined: CXXFLAGS += -O0 -g3 -fsanitize=undefined
undefined: re

noerr: CXXFLAGS := $(filter-out -Werror,$(CXXFLAGS))
noerr: re


$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo $(GREEN) $(NAME) created $(NO_COLOR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/Server $(OBJ_DIR)/Client $(OBJ_DIR)/Commands
	@echo $(YELLOW) Compiling $< $(NO_COLOR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo $(RED) $(NAME) objects deleted $(NO_COLOR)

fclean: clean
	@rm -f $(NAME)
	@echo $(RED) $(NAME) deleted $(NO_COLOR)

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)