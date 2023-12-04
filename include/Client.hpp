#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "IRC.hpp"


class Server;

class Client
{
    private:
        int             _socket;
        std::string     _nick;
        std::string     _user;
        std::string     _realname;
        std::string     _hostname;
        Server          &_server; // maybe not useful...
        bool            _registered;

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
        
        bool            isRegistered() const;

        void            setNick(const std::string& nick);
        void            setUser(std::vector<std::string> params);
        void            checkPassword(std::string password);
};

#endif
