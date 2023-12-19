NAME = ircserv

INCLUDES = -I./include/ -I./src/util/
SRC_DIR = src/
OBJ_DIR = obj/

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD $(INCLUDES)

SRC = Channel/Channel.cpp \
	Channel/ChannelMode.cpp \
	Client/Client.cpp \
	Client/ClientConnection.cpp \
	Client/ClientRegistration.cpp \
	Commands/Channel.cpp \
	Commands/Command.cpp \
	Commands/Connection.cpp \
	Commands/Help.cpp \
	Commands/Mode.cpp \
	Commands/Registration.cpp \
	Commands/Utils.cpp \
	main.cpp \
	Server/Server.cpp \
	Server/ServerKevent.cpp \
	Server/ServerLoop.cpp \
	Server/ServerUtils.cpp \
	util/util.cpp
	
SRC := $(addprefix $(SRC_DIR), $(SRC))
OBJ := $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
DEP := $(OBJ:%.o=%.d)

GREEN = "\033[32m
YELLOW = "\033[33m
RED = "\033[31m
NO_COLOR = \033[0m"

all: $(NAME)

debug: CXXFLAGS += -O0 -g3
debug: all

sanit: CXXFLAGS += -O0 -g3 -fsanitize=address
sanit: all

undefined: CXXFLAGS += -O0 -g3 -fsanitize=undefined
undefined: all

noerr: CXXFLAGS := $(filter-out -Werror,$(CXXFLAGS))
noerr: all

leak:
	@make debug
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --suppressions=vgsuppress.txt --log-file="valgrind.log" --gen-suppressions=all ./$(NAME) 6667 jambon


kill:
	kill $(shell pgrep $(NAME))

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo $(GREEN) $(NAME) created $(NO_COLOR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
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
