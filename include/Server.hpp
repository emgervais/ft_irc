#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <map>
#include <vector>
#include "IRC.hpp"
#include "NumericReplies.hpp"
#include "CHeaders.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
    private:
        static Server*          _instance;
        int                     _socket;
        int                     _port;
        int                     _maxClients;
        std::string             _pass;
        sockaddr_in             _addr;
        fd_set                  _readFdSet;
        fd_set                  _writeFdSet;
        std::map<int, Client*>  _clients;
        std::map<std::string, Channel*> _channels;
        struct kevent           _events[MAX_EVENTS];
        int                     _kqueue;
        char                    _buffer[BUFF_SIZE];

        void    setParams(int argc, char **argv);
        void    initSocket();
        void    initKqueue();
        
        void    serverQueue();
        void    registerNewClient();
        void    readFromClient(int socket);
        void    writeToClient(int socket);
    public:
        void    writeToClients(std::vector<int> sockets, const std::string& msg);
    private:
        void    closeClient(int socket, bool erase=true);
        // void    closeServer();
        void    handleMsg(int socket, ssize_t bytesRead);
        Server(void);
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);

    public:
        Server(int argc, char *arvg[]);
        ~Server();

        std::string     getPass() const;

        Channel*        getChannel(const std::string& name) const;
        void            createChannel(const std::string& name, const Client& client, const std::string& key);
        void            removeChannel(const std::string& name);

        bool            isNicknameTaken(const std::string& nickname);

        void    run();
};
void    initSignals();

#endif