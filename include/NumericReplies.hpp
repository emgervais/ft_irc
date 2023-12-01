#ifndef NUMERICSREPLIES_HPP
#define NUMERICSREPLIES_HPP

#include <string>
#include <vector>

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
#define ERR_ERRONEUSNICKNAME(nick) SERVER_NAME + "432 " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick) SERVER_NAME + "433 " + nick + " :Nickname is already in use"
#define ERR_NICKCOLLISION(nick, user, host) SERVER_NAME + "436 " + nick + " :Nickname collision KILL from " + user + "@" + host

// USER replies
// ERR_NEEDMOREPARAMS 461
// ERR_ALREADYREGISTRED 462

// PING replies
// ERR_NICKCOLLISION 436
#define ERR_NOORIGIN(nick) SERVER_NAME + "409 " + nick + " :No origin specified"

// PONG replies
// NONE

// OPER replies
// ERR_NEEDMOREPARAMS 461
// ERR_PASSWDMISMATCH 464
#define ERR_NOOPERHOST(nick) SERVER_NAME + "491 " + nick + " :No O-lines for your host"
#define RPL_YOUREOPER(nick) SERVER_NAME + "381 " + nick + " :You are now an IRC operator"

// QUIT replies
// NONE

// ERROR replies
// NONE

// JOIN replies
// ERR_NEEDMOREPARAMS 461
#define ERR_NOSUCHCHANNEL(nick, channel) SERVER_NAME + "403 " + nick + " " + channel + " :No such channel"
#define ERR_TOOMANYCHANNELS(nick, channel) SERVER_NAME + "405 " + nick + " " + channel + " :You have joined too many channels"
#define ERR_BADCHANNELKEY(nick, channel) SERVER_NAME + "475 " + nick + " " + channel + " :Cannot join channel (+k)"
#define ERR_BANNEDFROMCHAN(nick, channel) SERVER_NAME + "474 " + nick + " " + channel + " :Cannot join channel (+b)"
#define ERR_CHANNELISFULL(nick, channel) SERVER_NAME + "471 " + nick + " " + channel + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN(nick, channel) SERVER_NAME + "473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define ERR_BADCHANMASK(nick, channel) SERVER_NAME + "476 " + nick + " " + channel + " :Bad Channel Mask"
#define RPL_TOPIC(nick, channel, topic) SERVER_NAME + "332 " + nick + " " + channel + " :" + topic
#define RPL_TOPICWHOTIME(nick, channel, user, host, time) SERVER_NAME + "333 " + nick + " " + channel + " " + user + "!" + user + "@" + host + " " + time
#define RPL_NAMREPLY(nick, channel, users) SERVER_NAME + "353 " + nick + " = " + channel + " :" + users //See https://modern.ircdocs.horse/#rplnamreply-353
#define RPL_ENDOFNAMES(nick, channel) SERVER_NAME + "366 " + nick + " " + channel + " :End of /NAMES list"

// PART replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
#define ERR_NOTONCHANNEL(nick, channel) SERVER_NAME + "442 " + nick + " " + channel + " :You're not on that channel"

// TOPIC replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_NOTONCHANNEL 442
#define ERR_CHANOPRIVSNEEDED(nick, channel) SERVER_NAME + "482 " + nick + " " + channel + " :You're not channel operator"
#define RPL_NOTOPIC(nick, channel) SERVER_NAME + "331 " + nick + " " + channel + " :No topic is set"
// RPL_TOPIC 332
// RPL_TOPICWHOTIME 333

// NAMES replies
// RPL_NAMREPLY 353
// RPL_ENDOFNAMES 366

// LIST replies
#define RPL_LISTSTART(nick) SERVER_NAME + "321 " + nick + " Channel :Users Name"
#define RPL_LIST(nick, channel, usercount, topic) SERVER_NAME + "322 " + nick + " " + channel + " " + std::to_string(usercount) + " :" + topic
#define RPL_LISTEND(nick) SERVER_NAME + "323 " + nick + " :End of /LIST"

// INVITE replies
#define RPL_INVITING(nick, channel) SERVER_NAME + "341 " + nick + " " + channel
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_NOTONCHANNEL 442
// ERR_CHANOPRIVSNEEDED 482
#define ERR_USERONCHANNEL(nick, channel) SERVER_NAME + "443 " + nick + " " + channel + " :is already on channel"

// KICK replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_CHANOPRIVSNEEDED 482
#define ERR_USERNOTINCHANNEL(nick, channel) SERVER_NAME + "441 " + nick + " " + channel + " :They aren't on that channel"
// ERR_NOTONCHANNEL 442

#endif