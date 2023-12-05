#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "IRC.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class Server;
class Channel;

class Client
{
    private:
        int                             _socket;
        std::string                     _nick;
        std::string                     _user;
        std::string                     _realname;
        std::string                     _hostname;
        Server                          &_server;
        bool                            _registered;
        std::map<std::string, Channel*> _channels;

    public:
        //Client(void);
        Client(int socket, Server &server);
        // Client(Client const& rhs);              // Probably not useful since each client is unique
        // Client& operator=(Client const& rhs);   // Probably not useful since each client is unique
        ~Client();

        std::string     getNick() const;
        std::string     getUser() const;
        int             getSocket() const;
        std::string     getRealname() const;
        std::string     getHostname() const;
        std::vector<Channel*>   getChannels() const;

        bool            isOnChannel(const std::string& channel) const;
        bool            isOnChannel(const Channel& channel) const;
        bool            isRegistered() const;

        void            setNick(const std::string& nick);
        void            setUser(std::vector<std::string> params);
        void            checkPassword(std::string password);

        void            joinChannel(const std::string& channel);
        void            joinChannel(Channel *channel);
        void            partChannel(const std::string& channel);
        void            partChannel(Channel *channel);
        void            sendMessage(const std::string& msg, const std::string& channel);

        
};

#endif
