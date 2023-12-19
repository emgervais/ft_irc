#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Client;
class Server;

class Channel
{
    private:
        std::string             _name;
        std::string             _topic;
        std::vector<Client*>    _clients;
        Server                  &_server;
        std::string             _creationTime;
        std::string             _topicTime;
        std::map<std::string, std::vector<std::string> > _modes;

    public:
        Channel(const std::string& name, const Client& client, const Server& server, const std::string& key);
        ~Channel();

        void    addClient(Client *client);
        void    removeClient(Client *client, const std::string& reason, Client *kicker=NULL);
        bool    isClientOnChannel(const Client& client) const;
        bool    isClientOnChannel(const std::string& nick) const;
        bool    canJoin() const;

        void    setTopic(const std::string& topic);
        void    sendMessage(const std::string& msg, const std::string& sender="");
        void    sendMessageToOps(const std::string& msg, const std::string& sender="");

        std::string     getNamesReply(const Client& client) const;
        std::string     getNamesReply(const std::string& clientNick) const;
        std::string     getCreationTime() const;
        std::string     getName() const;
        std::string     getTopic() const;
        std::string     getTopicTime() const;
        int             getNbClients() const;

        std::string     getChanModes() const;
        void            removeAllModes(const Client& client);
        bool            isMode(const std::string& mode) const;
        bool            isMode(const std::string& mode, const std::string& param) const;
        void            addMode(const std::string& mode, const std::string& param);
        void            removeMode(const std::string& mode);
        void            removeMode(const std::string& mode, const std::string& param);
};

#endif