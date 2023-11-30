#ifndef NUMERICSREPLIES_HPP
#define NUMERICSREPLIES_HPP

#include <string>

const std::string SERVER_NAME = ":localhost ";

const char * const CMDS[] = {
    "PASS",
    "NICK",
    "USER",
    "PING",
    "PONG",
    "OPER",
    "QUIT",
    "ERROR",
    "JOIN",
    "PART",
    "TOPIC",
    "NAMES",
    "LIST",
    "INVITE",
    "KICK",
    "MODE",
    "PRIVMSG",
};

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

#define CHANNEL_MUST_START "&#"
#define CHANNEL_NOT_CONTAIN " ,^G"
#define CHANNEL_MAX_LEN 50

// Welcome replies
#define RPL_WELCOME(nick, user, host) SERVER_NAME + "001 " + nick + " :Welcome to the Internet Relay Network " + user + "!" + user + "@" + host
#define RPL_YOURHOST(nick) SERVER_NAME + "002 " + nick + " :Your host is " + SERVER_NAME + ", running version ircserv-0.0.1"
#define RPL_CREATED(nick) SERVER_NAME + "003 " + nick + " :This server was created " + CREATION_DATE
#define RPL_MYINFO(nick) SERVER_NAME + "004 " + nick + " " + SERVER_NAME + " ircserv 0.0.1 o itkol"
#define RPL_ISUPPORT(nick) SERVER_NAME + "005 " + nick + " PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=,,NICKLEN=9 CHANNELLEN=50 :are supported by this server"

// PASS replies
#define ERR_NEEDMOREPARAMS(nick) SERVER_NAME + "461 " + nick + " PASS :Not enough parameters"
#define ERR_ALREADYREGISTRED(nick) SERVER_NAME + "462 " + nick + " :You may not reregister"
#define ERR_PASSWDMISMATCH(nick) SERVER_NAME + "464 " + nick + " :Password incorrect"

// NICK replies
#define ERR_NONICKNAMEGIVEN(nick) SERVER_NAME + "431 " + nick + " :No nickname given"

#endif