#ifndef Command_HPP
#define Command_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "IRC.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server;
class Client;
class Command;
class Channel;

typedef void (Command::*cmdFunc)(void);

// A verifier : toutes commandes qui peuvent etre utilisees sur client meme
class Command
{
    private:
        Client                      &_client;
        Server                      &_server;
        std::string                  _cmd;
        std::vector<std::string>     _params;
        std::string                  _prefix;
        std::string                  _raw;

        Command(void);
        Command(Command const& rhs);
        Command& operator=(Command const& rhs);

        static std::map<const std::string, cmdFunc> _cmdHandler;
        
        void    cmdPass();
        void    cmdNick();
        void    cmdUser();
        void    cmdPing();
        void    cmdPong();
        void    cmdQuit();
        void    cmdJoin();
        void    cmdPart();
        void    cmdTopic();
        void    cmdNames();
        void    cmdList();
        void    cmdInvite();
        void    cmdKick();
        void    cmdMode();
        void    cmdPrivMsg();

        // void    cmdModeO(Channel *channel, char sign, const std::string &param);
        // void    cmdModeI(Channel *channel, char sign);
        // void    cmdModeK(Channel *channel, char sign, const std::string &param);
        // void    cmdModeL(Channel *channel, char sign, const std::string &param);
        // void    cmdModeT(Channel *channel, char sign);

    public:
        static void initCmdHandler();

        Command(Client &client, Server &server, std::string const&raw);
        ~Command();

        void    splitRawCommand();
        void    exec();
};


#endif