#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <map>
#include <vector>
#include "NumericReplies.hpp"
#include "CHeaders.hpp"
#include "Client.hpp"


class Server
{
    private:
        int                     _socket;
        int                     _port;
        int                     _maxClients;
        std::string             _pass;
        sockaddr_in             _addr;
        fd_set                  _readFdSet;
        fd_set                  _writeFdSet;
        std::map<int, Client>   _clients;
        struct kevent           _events[MAX_EVENTS];
        int                     _kqueue;
        char                    _buffer[BUFF_SIZE];

        void    setParams(int argc, char **argv);
        void    initSocket();
        void    initKqueue();
        
        void    serverQueue();
        void    registerNewClient();
        bool    isNicknameTaken(const std::string& nickname);
        void    sendErrorMessageToClient(int clientSocket, const std::string& errorMessage);
        void    readFromClient(int socket);
        void    writeToClient(int socket);
        Server(void);

    public:
        void cmdHandler(const std::string& data); // public for testing purposes
        Server(int argc, char *arvg[]);
        Server& operator=(const Server& rhs);
        ~Server();
        void    run();
};

#endif