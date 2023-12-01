#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Server;

class Client
{
    private:
        int             _socket;
        std::string     _nick;
        std::string     _user;
        std::string     _hostname;
        Server          &_server; // maybe not useful..
        bool            _registered;
        Client(void);

    public:
        Client(int socket, Server &server);
        Client(int socket, std::string nick, std::string user, Server &server);
        Client(Client const& rhs);
        Client& operator=(Client const& rhs);
        ~Client();

        std::string     getNick() const;
        std::string     getUser() const;
        int             getSocket() const;
        Server          getServer() const;
        bool            isRegistered() const;

        void            setNick(std::string nick);
        void            setUser(std::string user);
        void            setRegistered(bool registered);
};

#endif
