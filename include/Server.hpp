#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <map>
#include <vector>
#include "CHeaders.hpp"
#include "NumericReplies.hpp"
#include "Client.hpp"


class Server
{
    private:
        int                         _socket;
        int                         _port;
        int                         _maxClients;
        std::string                 _pass;
        sockaddr_in                 _addr;
        fd_set                      _readFdSet;
        fd_set                      _writeFdSet;
        // std::map<int, Client>       _clients;
        struct kevent               _events[MAX_EVENTS];
        int                         _kqueue;
        char                        _buffer[BUFF_SIZE];

        void    setParams(int argc, char **argv);
        void    initSocket();
        void    initFdSet();
        void    initKqueue();
        
        void    serverLoop();
        void    serverQueue();
        void    registerNewClient();
        void    readFromClient(int fd);
        void    writeToClient(int fd);

    public:
        Server(int argc, char *arvg[]);
        ~Server();
        void    run();
};

#endif