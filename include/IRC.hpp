#ifndef IRC_HPP
#define IRC_HPP

#include <string>
#include <map>

// à vérifier

// transformer en map (CMD: cmdHandler)
//const char * const CMDS[] = {"PASS","NICK","USER","PING","PONG","OPER","QUIT","ERROR","JOIN","PART","TOPIC","NAMES","LIST","INVITE","KICK","MODE","PRIVMSG",};

typedef void (*cmdHandler) (const std::string&, const std::string&, const std::string&);

// const std::map<std::string, cmdHandler> CMDS = {
//     {"PASS", cmdPass},
//     {"NICK", cmdNick},
//     {"USER", cmdUser},
//     {"PING", cmdPing},
//     {"PONG", cmdPong},
//     {"OPER", cmdOper},
//     {"QUIT", cmdQuit},
//     {"ERROR", cmdError},
//     {"JOIN", cmdJoin},
//     {"PART", cmdPart},
//     {"TOPIC", cmdTopic},
//     {"NAMES", cmdNames},
//     {"LIST", cmdList},
//     {"INVITE", cmdInvite},
//     {"KICK", cmdKick},
//     {"MODE", cmdMode},
//     {"PRIVMSG", cmdPrivmsg},
// };

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

#endif