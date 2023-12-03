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

        static std::map<const std::string, void (Command::*)()> createCmdHandler()
        {
            std::map<const std::string, void (Command::*)()> cmdHandler;

            cmdHandler["PASS"] = &Command::cmdPass;
            cmdHandler["NICK"] = &Command::cmdNick;
            cmdHandler["USER"] = &Command::cmdUser;
            // cmdHandler["PING"] = &Command::cmdPing;
            // cmdHandler["PONG"] = &Command::cmdPong;
            // cmdHandler["OPER"] = &Command::cmdOper;
            // cmdHandler["QUIT"] = &Command::cmdQuit;
            // cmdHandler["ERROR"] = &Command::cmdError;
            // cmdHandler["JOIN"] = &Command::cmdJoin;
            // cmdHandler["PART"] = &Command::cmdPart;
            // cmdHandler["TOPIC"] = &Command::cmdTopic;
            // cmdHandler["NAMES"] = &Command::cmdNames;
            // cmdHandler["LIST"] = &Command::cmdList;
            // cmdHandler["INVITE"] = &Command::cmdInvite;
            // cmdHandler["KICK"] = &Command::cmdKick;
            // cmdHandler["MODE"] = &Command::cmdMode;
            // cmdHandler["PRIVMSG"] = &Command::cmdPrivmsg;
            return cmdHandler;
        }

        void    cmdPass();
        void    cmdNick();
        void    cmdUser();
        // void    cmdPing();
        // void    cmdPong();
        // void    cmdOper();
        // void    cmdQuit();
        // void    cmdError();
        // void    cmdJoin();
        // void    cmdPart();
        // void    cmdTopic();
        // void    cmdNames();
        // void    cmdList();
        // void    cmdInvite();
        // void    cmdKick();
        // void    cmdMode();
        // void    cmdPrivmsg();

    public:
        Command(Client &client, Server &server, const std::string raw);
        ~Command();

        void    splitRawCommand();
        void    exec();
};

#endif