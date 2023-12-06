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

class Server;
class Client;
class Command;

typedef void (Command::*cmdFunc)(void);

class Command
{
    private:
        Client                      &_client;
        Server                      &_server;
        std::string                 _cmd;
        std::vector<std::string>    _params;
        std::string                 _prefix;
        std::string                 _raw;

        Command(void);
        Command(Command const& rhs);
        Command& operator=(Command const& rhs);

        static std::map<const std::string, cmdFunc> _cmdHandler;
        
        void    cmdPass();
        void    cmdNick();
        void    cmdUser();
        // void    cmdPing();
        // void    cmdPong();
        // void    cmdOper();
        void    cmdQuit();
        // void    cmdError();
        void    cmdJoin();
        // void    cmdPart();
        // void    cmdTopic();
        // void    cmdNames();
        // void    cmdList();
        // void    cmdInvite();
        // void    cmdKick();
        // void    cmdMode();
        // void    cmdPrivmsg();

    public:
        static void initCmdHandler();

        std::string contcatParams() const;
        Command(Client &client, Server &server, std::string const&raw);
        ~Command();

        void    splitRawCommand();
        void    exec();
        // static bool isCmd(std::string const& msg);
};


#endif