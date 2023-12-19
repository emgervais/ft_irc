#ifndef SERVER_HPP
#define SERVER_HPP

#include "IRC.hpp"
#include <sys/event.h>
#include <netinet/in.h>
#include <string>
#include <map>
#include <set>


#define SIGNAL_QTY 5
#define CHANGE_LIST_SIZE (SIGNAL_QTY + 2)
#define MAX_EVENTS (CHANGE_LIST_SIZE + MAX_CLIENTS * 2)

class Client;
class Channel;

class Server
{
    private:
        int                         _socket;
        int                         _port;
        std::string                 _pass;
        std::map<int, Client*>      _clients;
        std::map<std::string, Channel*> _channels;
        struct kevent               _events[MAX_EVENTS];
        struct kevent               _change[CHANGE_LIST_SIZE];
        char                        _buffer[BUFF_SIZE];
        int                         _kq;
        std::set<std::string>       _swearWords;
        std::map<char, char>        _equivalentChars;

        int     serverQueue(void);
        void    initSocket(void);
        void    initKqueue(void);
        void    bindSocket(sockaddr_in const& addr);
        void    addWriteKevent(void);
        void    editKevent(int socket, int filter, int flags, std::string msg);
        void    registerNewClient();
        void    readFromClient(int socket);
        void    writeToClient(int socket);
        void    readFromStdin(void);
        void    closeClient(int socket, bool eraseFromMap=true);
        void    handleMsg(int socket, ssize_t bytesRead);
        void    exitSignal(int sig);

        std::string     getClientIpAddr(int socket) const;
        void            loadSwearWords(void);
        void            initEquivalentChars(void);
        bool            checkEquivalentWords(std::string const& word);

        Server(void);
        Server(const Server& rhs);
        Server& operator=(const Server& rhs);
        Server(int port, std::string const& password);

    public:
        static Server& getInstance(int port, std::string const& password);
        void    run(void);
        ~Server(void);

        void            writeToClients(std::vector<int> sockets, const std::string& msg);
        std::string     getPass(void) const;
        Channel*        getChannel(const std::string& name) const;
        Client*         getClient(const std::string& nick) const;

        void            createChannel(const std::string& name, const Client& client, const std::string& key);
        void            removeChannel(const std::string& name);
        bool            isNicknameTaken(const std::string& nickname);

        std::string     getChannelReply(const std::string& name, const std::string& clientNick) const;
        std::vector<std::string>    getChannelsReply(const std::string& clientNick) const;
        std::string     censor(const std::string& str, Client *client);
        void            swearPolice(Client *c);
};

#endif