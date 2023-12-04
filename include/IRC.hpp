#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <map>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

#define FROM_SERVER(msg) std::cout << BLUE + std::string("Server: ") + RESET << msg << std::endl
#define FROM_CLIENT(msg) std::cout << YELLOW + std::string("Client: ") + RESET << msg << std::endl

#define NEW_CONNECTION(socket) std::cout << GREEN + std::string("New connection on socket ") + std::to_string(socket) + RESET << std::endl
#define CLOSE_CONNECTION(socket) std::cout << RED + std::string("Close connection on socket ") + std::to_string(socket) + RESET << std::endl

#define CREATION_DATE "2023-11-30"

#define BUFF_SIZE 1024
#define USAGE "Wrong usage : ./ircserv <port>(0-65535) <password>"

#define MAX_CLIENTS 10
#define MAX_EVENTS 128

#define MSG_MAX_LEN 512
#define CRLF "\r\n"

#define NICK_NOT_CONTAIN " ,.*!?@/"
#define NICK_NOT_START "0123456789$:#&"
#define NICK_MAX_LEN 9

#define USER_MAX_LEN 50
#define REALNAME_MAX_LEN 50
#define HOST_MAX_LEN 50

#define CHANNEL_MUST_START "&#"
#define CHANNEL_NOT_CONTAIN " ,^G"
#define CHANNEL_MAX_LEN 50

#endif
