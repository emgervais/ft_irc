#ifndef NUMERICSREPLIES_HPP
#define NUMERICSREPLIES_HPP

#include <string>

const std::string SERVER_NAME = ":irc.localhost ";

#define CRLF "\r\n"

// Replies
#define RPL_WELCOME(nick, user, host) SERVER_NAME + "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + CRLF
#define RPL_YOURHOST(nick) SERVER_NAME + "002 " + nick + " :Your host is " + SERVER_NAME + ", running version ircserv-0.0.1" + CRLF
#define RPL_CREATED(nick) SERVER_NAME + "003 " + nick + " :This server was created " + CREATION_DATE + CRLF
#define RPL_MYINFO(nick) SERVER_NAME + "004 " + nick + " " + SERVER_NAME + " 0.0.1 i linst :klo" + CRLF
#define RPL_ISUPPORT(nick) SERVER_NAME + "005 " + nick + " CASEMAPPING=ascii CHANMODES=o,k,l,inst" + CHANNELLEN + " CHANTYPES=#" + HOSTLEN + LINELEN + MAXTARGETS + NICKLEN + " PREFIX=(o)@" + TOPICLEN + USERLEN + " USERMODES=,,,i :are supported by this server" + CRLF
#define RPL_PINGUSE(nick) SERVER_NAME + "650 " + nick + " PING :<cookie> [<servername>]" + CRLF
#define RPL_PONG(nick, server, token) SERVER_NAME + " PONG " + nick + server + " :" + token + CRLF
#define RPL_YOUREOPER(nick) SERVER_NAME + "381 " + nick + " :You are now an IRC operator" + CRLF
#define RPL_QUIT(user, host, message) "ERROR :Closing link: (" + user + "@" + host + ") [Quit: " + message + "]" + CRLF
#define RPL_TOPIC(nick, channel, topic) SERVER_NAME + "332 " + nick + " " + channel + " :" + topic + CRLF
#define RPL_TOPICWHOTIME(nick, channel, user, host, creationtime) SERVER_NAME + "333 " + nick + " " + channel + " " + user + "!" + user + "@" + host + " " + creationtime + CRLF
#define RPL_NAMREPLY(nick, channel, users) SERVER_NAME + "353 " + nick + " = " + channel + " :" + users + CRLF
#define RPL_ENDOFNAMES(nick, channel) SERVER_NAME + "366 " + nick + " " + channel + " :End of /NAMES list" + CRLF
#define RPL_JOIN(prefix, channel) prefix + " JOIN " + channel + CRLF
#define RPL_PART(prefix, channel, reason) prefix + " PART " + channel + (reason.empty() ? "" : " :" + reason) + CRLF
#define RPL_NOTOPIC(nick, channel) SERVER_NAME + "331 " + nick + " " + channel + " :No topic is set" + CRLF
#define RPL_SETTOPIC(prefix, channel, topic) prefix + " TOPIC " + channel + " :" + topic + CRLF
#define RPL_LISTSTART(nick) SERVER_NAME + "321 " + nick + " Channel :Users Name" + CRLF
#define RPL_LIST(nick, channel, userCount, topic) SERVER_NAME + "322 " + nick + " " + channel + " " + std::to_string(userCount) + " :" + topic + CRLF
#define RPL_LISTEND(nick) SERVER_NAME + "323 " + nick + " :End of /LIST" + CRLF
#define RPL_INVITING(nick, nickInvite, channel) SERVER_NAME + "341 " + nick + " " + nickInvite + " " + channel + CRLF
#define RPL_INVITE(nick, nickInvite, channel) ":" + nick + " INVITE " + nickInvite + " " + channel + CRLF
#define RPL_KICK(prefix, channel, nickKick, reason) prefix + " KICK " + channel + " " + nickKick + " :" + reason + CRLF
#define RPL_AWAY(prefix, target, message) SERVER_NAME + "301 " + nick + " " + target + " :" + message + CRLF
#define RPL_CHANNELMODEIS(nick, channel, modes) SERVER_NAME + "324 " + nick + " " + channel + " :" + modes + CRLF
#define RPL_CREATIONTIME(nick, channel, creationtime) SERVER_NAME + "329 " + nick + " " + channel + " " + creationtime + CRLF
#define RPL_PRIVMSG(prefix, target, message) prefix + " PRIVMSG " + target + " :" + message + CRLF
#define RPL_MODE(prefix, target, modes) prefix + " MODE " + target + " " + modes + CRLF
#define RPL_HELPSTART(nick, command, msg) SERVER_NAME + "704 " + nick + " " + command + " :" + command + CRLF
#define RPL_HELPTXT(nick, command, msg) SERVER_NAME + "705 " + nick + " " + command + " :" + msg + CRLF
#define RPL_ENDOFHELP(nick, command) SERVER_NAME + "706 " + nick + " " + command + " :End of /HELP" + CRLF
#define RPL_UMODEIS(nick, modes) SERVER_NAME + "221 " + nick + " " + modes + CRLF

// Errors
#define ERR_PASSWREQ(nick) SERVER_NAME + "461 " + nick + " PASS :Password required" + CRLF
#define ERR_NEEDMOREPARAMS(nick, command) SERVER_NAME + "461 " + nick + " " + command + " :Not enough parameters" + CRLF
#define ERR_ALREADYREGISTRED(nick) SERVER_NAME + "462 " + nick + " :You may not reregister" + CRLF
#define ERR_PASSWDMISMATCH(nick) SERVER_NAME + "464 " + nick + " :Password incorrect" + CRLF
#define ERR_NOSUCHNICK(nick, nickTarget) SERVER_NAME + "401 " + nick + " " + nickTarget + " :No such nick/channel" + CRLF
#define ERR_NONICKNAMEGIVEN(nick) SERVER_NAME + "431 " + nick + " :No nickname given" + CRLF
#define ERR_ERRONEUSNICKNAME(nick) SERVER_NAME + "432 " + nick + " :Erroneous nickname" + CRLF
#define ERR_NICKNAMEINUSE(nick) SERVER_NAME + "433 " + nick + " :Nickname is already in use" + CRLF
#define ERR_NICKCOLLISION(nick, user, host) SERVER_NAME + "436 " + nick + " :Nickname collision KILL from " + user + "@" + host + CRLF
#define ERR_ERRONUSERNAME(nick) SERVER_NAME + "468 " + nick + " :Erroneous username" + CRLF
#define ERR_NOORIGIN(nick) SERVER_NAME + "409 " + nick + " :No origin specified" + CRLF
#define ERR_NOOPERHOST(nick) SERVER_NAME + "491 " + nick + " :No O-lines for your host" + CRLF
#define ERR_NOSUCHCHANNEL(nick, channel) SERVER_NAME + "403 " + nick + " " + channel + " :No such channel" + CRLF
#define ERR_TOOMANYCHANNELS(nick, channel) SERVER_NAME + "405 " + nick + " " + channel + " :You have joined too many channels" + CRLF
#define ERR_BADCHANNELKEY(nick, channel) SERVER_NAME + "475 " + nick + " " + channel + " :Cannot join channel (+k)" + CRLF
#define ERR_BANNEDFROMCHAN(nick, channel) SERVER_NAME + "474 " + nick + " " + channel + " :Cannot join channel (+b)" + CRLF
#define ERR_CHANNELISFULL(nick, channel) SERVER_NAME + "471 " + nick + " " + channel + " :Cannot join channel (+l)" + CRLF
#define ERR_INVITEONLYCHAN(nick, channel) SERVER_NAME + "473 " + nick + " " + channel + " :Cannot join channel (+i)" + CRLF
#define ERR_BADCHANMASK(nick, channel) SERVER_NAME + "476 " + nick + " " + channel + " :Bad Channel Mask" + CRLF
#define ERR_NOTONCHANNEL(nick, channel) SERVER_NAME + "442 " + nick + " " + channel + " :You're not on that channel" + CRLF
#define ERR_CHANOPRIVSNEEDED(nick, channel) SERVER_NAME + "482 " + nick + " " + channel + " :You're not channel operator" + CRLF
#define ERR_USERONCHANNEL(nick, nickInvite, channel) SERVER_NAME + "443 " + nick + " " + nickInvite + " " + channel + " :is already on channel" + CRLF
#define ERR_USERNOTINCHANNEL(nick, nickKick, channel) SERVER_NAME + "441 " + nick + " " + nickKick + channel + " :They aren't on that channel" + CRLF
#define ERR_UNKNOWNCOMMAND(nick, command) SERVER_NAME + "421 " + nick + " " + command + " :Unknown command" + CRLF
#define ERR_INPUTTOOLONG(nick) SERVER_NAME + "417 " + nick + " :Input line too long" + CRLF
#define ERR_NOSUCHSERVER(nick, server) SERVER_NAME + "402 " + nick + " " + server + " :No such server" + CRLF
#define ERR_CANNOTSENDTOCHAN(nick, channel) SERVER_NAME + "404 " + nick + " " + channel + " :Cannot send to channel" + CRLF
#define ERR_TOOMANYTARGETS(nick, command) SERVER_NAME + "407 " + nick + " " + command + " :Too many recipients" + CRLF
#define ERR_NORECIPIENT(nick, command) SERVER_NAME + "411 " + nick + " " + command + " :No recipient given (" + command + ")" + CRLF
#define ERR_NOTEXTTOSEND(nick) SERVER_NAME + "412 " + nick + " :No text to send" + CRLF
#define ERR_NOTOPLEVEL(nick, mask) SERVER_NAME + "413 " + nick + " " + mask + " :No toplevel domain specified" + CRLF
#define ERR_WILDTOPLEVEL(nick, mask) SERVER_NAME + "414 " + nick + " " + mask + " :Wildcard in toplevel domain" + CRLF
#define ERR_UNKNOWNMODE(nick, mode) SERVER_NAME + "472 " + nick + " " + mode + " :is not a recognised channel mode." + CRLF
#define ERR_SPECIFYLIMIT(nick) SERVER_NAME + "696 " + nick + " l * :You must specify a parameter for the limit mode. Syntax: <limit>." + CRLF
#define ERR_SPECIFYKEY(nick) SERVER_NAME + "696 " + nick + " k * :You must specify a parameter for the key mode. Syntax: <key>." + CRLF
#define ERR_SPECIFYOP(nick) SERVER_NAME + "696 " + nick + " o * :You must specify a parameter for the op mode. Syntax: <nick>." + CRLF
#define ERR_INVALIDKEY(nick, channel, key) SERVER_NAME + "696 " + nick + channel + " k " + key + " :Invalid key mode parameter. Syntax: <key>." + CRLF
#define ERR_INVALIDLIMIT(nick, channel, limit) SERVER_NAME + "696 " + nick + channel + " l " + limit + " :Invalid limit mode parameter. Syntax: <limit>." + CRLF
#define ERR_HELPNOTFOUND(nick, topic) SERVER_NAME + "524 " + nick + topic + " :There is no help for the topic you searched for. Please try again." + CRLF
#define ERR_USERSDONTMATCH(nick) SERVER_NAME + "502 " + nick + " :Cant change mode for other users" + CRLF
#define ERR_UMODEUNKNOWNFLAG(nick, mode) SERVER_NAME + "501 " + nick + " " + mode + " :is not a recognised user mode." + CRLF

#endif