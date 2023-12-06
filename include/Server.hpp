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
        int                     _socket;
        int                     _port;
        std::string             _pass;
        int                     _maxClients;
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
        void    editKevent(int socket, int filter, int flags, std::string msg);
        void    registerNewClient();
        void    readFromClient(int socket);
        void    writeToClient(int socket);
        void    closeClient(int socket, bool erase=true);
        void    handleMsg(int socket, ssize_t bytesRead);

        Server(void);
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        Server(int port, std::string const& password);
    public:
        static Server& getInstance(int port, std::string const& password);
        void    run();
        ~Server();

        void            writeToClients(std::vector<int> sockets, const std::string& msg);
        std::string     getPass() const;
        Channel*        getChannel(const std::string& name) const;
        void            createChannel(const std::string& name, const Client& client, const std::string& key);
        void            removeChannel(const std::string& name);
        bool            isNicknameTaken(const std::string& nickname);
};

#endif