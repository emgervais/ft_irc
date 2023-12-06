#ifndef NUMERICSREPLIES_HPP
#define NUMERICSREPLIES_HPP

#include <string>
#include "IRC.hpp"

const std::string SERVER_NAME = ":irc.localhost ";

// Welcome replies
#define RPL_WELCOME(nick, user, host) SERVER_NAME + "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + CRLF
#define RPL_YOURHOST(nick) SERVER_NAME + "002 " + nick + " :Your host is " + SERVER_NAME + ", running version ircserv-0.0.1" + CRLF
#define RPL_CREATED(nick) SERVER_NAME + "003 " + nick + " :This server was created " + CREATION_DATE + CRLF
#define RPL_MYINFO(nick) SERVER_NAME + "004 " + nick + " " + SERVER_NAME + " ircserv 0.0.1 o itkol" + CRLF
#define RPL_ISUPPORT(nick) SERVER_NAME + "005 " + nick + " PREFIX=(o)@ CHANTYPES=# CHANMODES=iksn CHANLIMIT=#:10 NICKLEN=9 CHANNELLEN=50 TOPICLEN=100 KICKLEN=50 :are supported by this server" + CRLF

// PASS replies
#define ERR_PASSWREQ(nick) SERVER_NAME + "461 " + nick + " PASS :Password required" + CRLF
#define ERR_NEEDMOREPARAMS(nick, command) SERVER_NAME + "461 " + nick + " " + command + " :Not enough parameters" + CRLF
#define ERR_ALREADYREGISTRED(nick) SERVER_NAME + "462 " + nick + " :You may not reregister" + CRLF
#define ERR_PASSWDMISMATCH(nick) SERVER_NAME + "464 " + nick + " :Password incorrect" + CRLF

// NICK replies
#define ERR_NONICKNAMEGIVEN(nick) SERVER_NAME + "431 " + nick + " :No nickname given" + CRLF
#define ERR_ERRONEUSNICKNAME(nick) SERVER_NAME + "432 " + nick + " :Erroneous nickname" + CRLF
#define ERR_NICKNAMEINUSE(nick) SERVER_NAME + "433 " + nick + " :Nickname is already in use" + CRLF
#define ERR_NICKCOLLISION(nick, user, host) SERVER_NAME + "436 " + nick + " :Nickname collision KILL from " + user + "@" + host + CRLF

// USER replies
// ERR_NEEDMOREPARAMS 461
// ERR_ALREADYREGISTRED 462
#define ERR_ERRONUSERNAME(nick) SERVER_NAME + "468 " + nick + " : USER Your username is not valid" + CRLF

// PING replies
// ERR_NICKCOLLISION 436
#define ERR_NOORIGIN(nick) SERVER_NAME + "409 " + nick + " :No origin specified" + CRLF

// PONG replies
// NONE

// OPER replies
// ERR_NEEDMOREPARAMS 461
// ERR_PASSWDMISMATCH 464
#define ERR_NOOPERHOST(nick) SERVER_NAME + "491 " + nick + " :No O-lines for your host" + CRLF
#define RPL_YOUREOPER(nick) SERVER_NAME + "381 " + nick + " :You are now an IRC operator" + CRLF

// QUIT replies
#define RPL_QUIT(user, host, message) "ERROR :Closing link: (" + user + "@" + host + ") [Quit: " + message + "]" + CRLF

// ERROR replies
// NONE

// JOIN replies
// ERR_NEEDMOREPARAMS 461
#define ERR_NOSUCHCHANNEL(nick, channel) SERVER_NAME + "403 " + nick + " " + channel + " :No such channel" + CRLF
#define ERR_TOOMANYCHANNELS(nick, channel) SERVER_NAME + "405 " + nick + " " + channel + " :You have joined too many channels" + CRLF
#define ERR_BADCHANNELKEY(nick, channel) SERVER_NAME + "475 " + nick + " " + channel + " :Cannot join channel (+k)" + CRLF
#define ERR_BANNEDFROMCHAN(nick, channel) SERVER_NAME + "474 " + nick + " " + channel + " :Cannot join channel (+b)" + CRLF
#define ERR_CHANNELISFULL(nick, channel) SERVER_NAME + "471 " + nick + " " + channel + " :Cannot join channel (+l)" + CRLF
#define ERR_INVITEONLYCHAN(nick, channel) SERVER_NAME + "473 " + nick + " " + channel + " :Cannot join channel (+i)" + CRLF
#define ERR_BADCHANMASK(nick, channel) SERVER_NAME + "476 " + nick + " " + channel + " :Bad Channel Mask" + CRLF
#define RPL_TOPIC(nick, channel, topic) SERVER_NAME + "332 " + nick + " " + channel + " :" + topic + CRLF
#define RPL_TOPICWHOTIME(nick, channel, user, host, time) SERVER_NAME + "333 " + nick + " " + channel + " " + user + "!" + user + "@" + host + " " + time + CRLF
#define RPL_NAMREPLY(nick, channel, users) SERVER_NAME + "353 " + nick + " = " + channel + " :" + users + CRLF //See https://modern.ircdocs.horse/#rplnamreply-353
#define RPL_ENDOFNAMES(nick, channel) SERVER_NAME + "366 " + nick + " " + channel + " :End of /NAMES list" + CRLF
#define RPL_JOIN(nick, user, host, channel) ":" + nick + "!" + user + "@" + host + " JOIN " + channel + CRLF

// PART replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
#define ERR_NOTONCHANNEL(nick, channel) SERVER_NAME + "442 " + nick + " " + channel + " :You're not on that channel" + CRLF
#define RPL_PART(nick, user, host, channel, reason) ":" + nick + "!" + user + "@" + host + " PART " + channel + (reason.empty() ? "" : " :" + reason) + CRLF

// TOPIC replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_NOTONCHANNEL 442
#define ERR_CHANOPRIVSNEEDED(nick, channel) SERVER_NAME + "482 " + nick + " " + channel + " :You're not channel operator" + CRLF
#define RPL_NOTOPIC(nick, channel) SERVER_NAME + "331 " + nick + " " + channel + " :No topic is set" + CRLF
// RPL_TOPIC 332
// RPL_TOPICWHOTIME 333

// NAMES replies
// RPL_NAMREPLY 353
// RPL_ENDOFNAMES 366

// LIST replies
#define RPL_LISTSTART(nick) SERVER_NAME + "321 " + nick + " Channel :Users Name" + CRLF
#define RPL_LIST(nick, channel, usercount, topic) SERVER_NAME + "322 " + nick + " " + channel + " " + std::to_string(usercount) + " :" + topic + CRLF
#define RPL_LISTEND(nick) SERVER_NAME + "323 " + nick + " :End of /LIST" + CRLF

// INVITE replies
#define RPL_INVITING(nick, channel) SERVER_NAME + "341 " + nick + " " + channel + " :Inviting " + nick + " to channel " + channel + CRLF
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_NOTONCHANNEL 442
// ERR_CHANOPRIVSNEEDED 482
#define ERR_USERONCHANNEL(nick, channel) SERVER_NAME + "443 " + nick + " " + channel + " :is already on channel" + CRLF

// KICK replies
// ERR_NEEDMOREPARAMS 461
// ERR_NOSUCHCHANNEL 403
// ERR_CHANOPRIVSNEEDED 482
#define ERR_USERNOTINCHANNEL(nick, channel) SERVER_NAME + "441 " + nick + " " + channel + " :They aren't on that channel" + CRLF
// ERR_NOTONCHANNEL 442

// OTHER replies
#define ERR_UNKNOWNCOMMAND(nick, command) SERVER_NAME + "421 " + nick + " " + command + " :Unknown command" + CRLF
#define ERR_INPUTTOOLONG(nick) SERVER_NAME + "417 " + nick + " :Input line too long" + CRLF

// PRIVMSG replies
#endif