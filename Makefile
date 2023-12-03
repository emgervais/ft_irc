NAME = ircserv

CXX = clang++
INCLUDES = -I./include
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD $(INCLUDES)

SRC = Server/Server.cpp Server/ServerCmdHandler.cpp Server/UnixSignals.cpp\
	Client/Client.cpp Client/ClientRegistration.cpp \
	Commands/Command.cpp Commands/Registration.cpp \
	main.cpp \

SRC_DIR = src/
OBJ_DIR = obj/

SRC := $(addprefix $(SRC_DIR), $(SRC))
OBJ := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
DEP := $(OBJ:%.o=%.d)

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
	@echo "\033[32m$(NAME) created\033[0m"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/Server $(OBJ_DIR)/Client $(OBJ_DIR)/Commands
	@echo "\033[33mCompiling $<\033[0m"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[31m$(NAME) objects deleted\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m$(NAME) deleted\033[0m"

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)