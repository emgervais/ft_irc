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
        Server          &_server; // maybe not useful..
        int             _registrationStep;

        std::string     _reply;
        bool            _newlyRegistered;
    public:
        //Client(void);
        Client(int socket, Server &server);
        // Client(Client const& rhs);              // Probably not useful since each client is unique
        // Client& operator=(Client const& rhs);   // Probably not useful since each client is unique
        ~Client();

        std::string     getNick() const;
        std::string     getUser() const;
        int             getSocket() const;
        std::string     getReply();
        
        bool            isRegistered() const;

        bool            isNickValid(const std::string& nick);
        bool            isUserValid(const std::vector<std::string>& params);
        bool            isPassValid(const std::string& password);

        bool            setNick(const std::string& nick);
        bool            setUser(std::vector<std::string> params);
        bool            setRealname(std::string realname);
        bool            setHostname(std::string hostname);
        bool            checkPassword(std::string password);

        void            setNewlyRegistered(bool newlyRegistered);
        bool            isNewlyRegistered() const;
};

#endif
