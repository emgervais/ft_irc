#include "Channel.hpp"
#include "../Client/Client.hpp"
#include "../util/util.hpp"

bool    Channel::isMode(const std::string& mode) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode && it->second.size() > 0)
            return true;
    }
    return false;
}

bool    Channel::isMode(const std::string& mode, const std::string& param) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            std::vector<std::string>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                if (toUpper(*it2) == toUpper(param))
                    return true;
        }
    }
    return false;
}

void    Channel::addMode(const std::string& mode, const std::string& param)
{
    if (isMode(mode, param))
        return;
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            it->second.push_back(param);
            return;
        }
    }
    std::vector<std::string> v;
    v.push_back(param);
    _modes[mode] = v;
}

void    Channel::removeMode(const std::string& mode)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            if (mode != "o")
                _modes.erase(it);
            return;
        }
    }
}

void    Channel::removeMode(const std::string& mode, const std::string& param)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            std::vector<std::string>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
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

void    Channel::removeAllModes(const Client& client)
{
    char    mode[3] = {'o', 'i', '\0'};

    for (int i = 0; mode[i]; ++i)
        removeMode(std::string(1, mode[i]), client.getNick());
}

std::string     Channel::getChanModes() const
{
    std::string reply = "+";
    std::string params;

    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first != "o")
        {
            reply += it->first;
            if (it->first == "k" || it->first == "l")
            {
                if (it->second.size() > 0)
                    params += " " + it->second[0];
            }
        }
    }
    if (!params.empty())
    {
        ssize_t pos = params.find_last_of(' ');
        params.insert(pos + 1, ":");
    }
    return (reply + params);
}
