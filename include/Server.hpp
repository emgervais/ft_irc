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

#define SIGNAL_QTY 5
#define CHANGE_LIST_SIZE (SIGNAL_QTY + 1)
#define MAX_EVENTS (CHANGE_LIST_SIZE + MAX_CLIENTS * 2)


class Server
{
    private:
        int                     _socket;
        int                     _port;
        std::string             _pass;
        size_t                  _maxClients;
        sockaddr_in             _addr;
        fd_set                  _readFdSet;
        fd_set                  _writeFdSet;
        std::map<int, Client*>  _clients;
        std::map<std::string, Channel*> _channels;
        struct kevent           _events[MAX_EVENTS];
        struct kevent           _change[CHANGE_LIST_SIZE];
        char                    _buffer[BUFF_SIZE];
        int                     _kq;

        int     serverQueue();
        void    initSocket();
        void    initKqueue();
        void    addWriteKevent();
        void    editKevent(int socket, int filter, int flags, std::string msg);
        void    registerNewClient();
        void    readFromClient(int socket);
        void    writeToClient(int socket);
        void    closeClient(int socket);
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
        Client*         getClient(const std::string& nick) const;

        void            createChannel(const std::string& name, const Client& client, const std::string& key);
        void            removeChannel(const std::string& name);
        bool            isNicknameTaken(const std::string& nickname);

        std::string     getChannelReply(const std::string& name, const std::string& clientNick) const;
        std::vector<std::string>    getChannelsReply(const std::string& clientNick) const;
};

#endif