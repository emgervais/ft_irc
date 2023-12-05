#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "IRC.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel
{
    private:
        std::string             _name;
        std::string             _topic;
        std::vector<Client*>    _clients;
        Server                  &_server;
        std::map<std::string, std::vector<std::string> > _modes;

    public:
        Channel(const std::string& name, const Client& client, const Server& server);
        ~Channel();

        void    addClient(Client *client);
        void    removeClient(Client *client);
        void    setTopic(const std::string& topic);
        void    sendMessage(const std::string& msg);

        std::string     getName() const;
        std::string     getTopic() const;
        std::vector<Client*>    getClients() const;
        bool    isClientInChannel(const Client& client) const;
        bool    isOp(const Client& client) const;
        bool    isInvited(const Client& client) const;

        template <typename T>
        void    addMode(const std::string& mode, const T& param)
        {
            for (typename std::map<std::string, std::vector<std::string> >::iterator it = _modes.begin(); it != _modes.end(); ++it)
            {
                if (it->first == mode)
                {
                    for (typename std::vector<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        if (*it2 == param)
                            return;
                    }
                    it->second.push_back(param);
                    return;
                }
            }
            std::vector<std::string>    tmp;
            tmp.push_back(param);
            _modes[mode] = tmp;
        }

        template <typename T>
        void    removeMode(const std::string& mode, const T& param)
        {
            for (typename std::map<std::string, std::vector<std::string> >::iterator it = _modes.begin(); it != _modes.end(); ++it)
            {
                if (it->first == mode)
                {
                    for (typename std::vector<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        if (*it2 == param)
                        {
                            it->second.erase(it2);
                            return;
                        }
                    }
                }
            }
        }
        
        template <typename T>
        bool    isMode(const std::string& mode, const T& param) const
        {
            for (typename std::map<std::string, std::vector<std::string> >::const_iterator it = _modes.begin(); it != _modes.end(); ++it)
            {
                if (it->first == mode)
                {
                    for (typename std::vector<std::string>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                    {
                        if (*it2 == param)
                            return (true);
                    }
                }
            }
            return (false);
        }

};

#endif