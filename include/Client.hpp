#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client
{
    private:
        int             _socket;
        std::string     _nick;
        std::string     _user;
        std::string     _hostname;
        bool            _registered;
        Server          &_server;

    public:
        Client(int socket, Server &server);
        Client(int socket, std::string nick, std::string user, Server &server);
        ~Client();

        std::string     getNick();
        std::string     getUser();
        int             getSocket();
        Server          getServer();
        bool            isRegistered();

        void            setNick(std::string nick);
        void            setUser(std::string user);
        void            setRegistered(bool registered);
};

#endif
