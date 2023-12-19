#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <queue>
#include <map>

class Channel;
class Server;

class Client
{
    private:
        int                             _socket;
        std::string                     _nick;
        std::string                     _user;
        std::string                     _realname;
        std::string                     _hostname;
        std::string                     _ping;
        bool                            _waitingForPong;
        Server                          &_server;
        bool                            _registered;
        bool                            _passChecked;
        bool                            _closing;
        std::map<std::string, Channel*> _channels;
        int                             _warnings;
        std::string                     _mode;
        std::string                     _buff;

        std::queue<std::string>         _sendQueue;
    public:
        Client(int socket, Server &server);
        ~Client();

        std::string     getPrefix() const;
        std::string     getNick() const;
        std::string     getUser() const;
        int             getSocket() const;
        std::string     getRealname() const;
        std::string     getHostname() const;
        std::string     getPing() const;
        std::string     addBuff(const std::string input);
        void            chopBuff();

        void            setClosing();
        bool            isClosing() const;
        bool            isRegistered() const;
        void            setRegistered();
        bool            isWaitingForPong() const;
        void            disableWaitingForPong();

        void            setNick(const std::string& nick);
        void            setUser(std::vector<std::string> params);
        void            checkPassword(std::string password);

        void            joinChannel(const std::string& channel, const std::string& key);
        void            partChannel(const std::string& channel, const std::string& reason);
        void            partAllChannels();

        void            sendMessage(std::vector<std::string> targets, const std::string& message);

        std::string     getReply() const;
        size_t          getRepliesQty() const;
        void            addReply(const std::string& reply);
        void            removeReply();
        void            addWarning();
        int             getWarning();

        void            setMode(const std::string& mode);
        bool            isInvisible() const;
        std::string     getMode() const;
};

#endif
