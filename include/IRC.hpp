#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <string>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET_COLOR "\033[0m"
#define ERROR -1
#define SWEAR_WORDS_PATH "data/swearWords.txt"
#define BOT std::string("Kevin")

#define FROM_SERVER(msg) std::cout << BLUE + std::string("Server: ") + RESET_COLOR << msg << std::endl
#define FROM_CLIENT(msg) std::cout << YELLOW + std::string("Client: ") + RESET_COLOR << msg << std::endl

#define NEW_CONNECTION_MSG(socket) std::cout << GREEN + std::string("New connection on socket ") + std::to_string(socket) + RESET_COLOR << std::endl
#define CLOSE_CONNECTION_MSG(socket) std::cout << RED + std::string("Close connection on socket ") + std::to_string(socket) + RESET_COLOR << std::endl

#define CREATION_DATE "23:06:04 Dec 13 2023"

#define BUFF_SIZE 1024
#define USAGE "Wrong usage : ./ircserv <port>(0-65535) <password>"

#define MAX_CLIENTS 1024

#define NICK_NOT_CONTAIN " ,.*!?@/"
#define NICK_NOT_START "0123456789$:#&+-"

#define CHANNEL_MAX_LEN 50
#define HOST_MAX_LEN 50
#define MSG_MAX_LEN 512
#define MAX_TARGETS 20
#define NICK_MAX_LEN 30
#define TOPIC_MAX_LEN 100
#define USER_MAX_LEN 50
#define REALNAME_MAX_LEN 50

#define MAX_WARNINGS 3

#define CHANNELLEN std::string(" CHANNELLEN") + std::to_string(CHANNEL_MAX_LEN)
#define HOSTLEN std::string(" HOSTLEN") + std::to_string(HOST_MAX_LEN)
#define LINELEN std::string(" LINELEN") + std::to_string(MSG_MAX_LEN)
#define MAXTARGETS std::string(" MAXTARGETS") + std::to_string(MAX_TARGETS)
#define NICKLEN std::string(" NICKLEN") + std::to_string(NICK_MAX_LEN)
#define TOPICLEN std::string(" TOPICLEN") + std::to_string(TOPIC_MAX_LEN)
#define USERLEN std::string(" USERLEN") + std::to_string(USER_MAX_LEN)

#endif
