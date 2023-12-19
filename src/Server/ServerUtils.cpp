#include "Server.hpp"
#include "../util/util.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "NumericReplies.hpp"
#include "IRC.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <string>

// -- misc ----
std::string Server::getPass() const
{
    return _pass;
}

bool Server::isNicknameTaken(const std::string& nickname)
{
    std::map<int, Client*>::const_iterator it;
    int i = 0;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (!it->second)
            std::cout << "Client " << i << " is NULL" << std::endl;
        else if (toUpper(it->second->getNick()) == toUpper(nickname))
            return true;
        i++;
    }
    return false;
}

std::string Server::getClientIpAddr(int socket) const
{
    struct sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);
    getpeername(socket, (struct sockaddr *)&addr, &addrSize);
    return inet_ntoa(addr.sin_addr);
}

Channel* Server::getChannel(const std::string& name) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    return NULL;
}

Client* Server::getClient(const std::string& nick) const
{
    std::map<int, Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNick() == nick)
            return it->second;
    }
    return NULL;
}

void Server::createChannel(const std::string& name, const Client& client, const std::string& key)
{
    _channels[name] = new Channel(name, client, *this, key);
}

void Server::removeChannel(const std::string& name)
{
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
        {
            delete it->second;
            _channels.erase(it);
            return;
        }
    }
}

std::string Server::getChannelReply(const std::string& name, const std::string& clientNick) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name && (it->second->isClientOnChannel(clientNick) || it->second->isMode("s") == false))
            return it->second->getNamesReply(clientNick);
    }
    return "";
}

std::vector<std::string> Server::getChannelsReply(const std::string& clientNick) const
{
    std::vector<std::string> channelsList;
    std::map<std::string, Channel*>::const_iterator it;

    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second->isClientOnChannel(clientNick) || it->second->isMode("s") == false)
            channelsList.push_back(it->second->getNamesReply(clientNick));
    }
    return channelsList;
}

void    Server::writeToClients(std::vector<int> sockets, const std::string& msg)
{
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (std::find(sockets.begin(), sockets.end(), it->first) == sockets.end())
            it->second->addReply(msg);
}

void Server::loadSwearWords()
{
    std::ifstream file(SWEAR_WORDS_PATH);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error opening SWEAR_WORDS_PATH: " << SWEAR_WORDS_PATH << std::endl;
        return;
    }
    while (std::getline(file, line))
        if (!line.empty())
            _swearWords.insert(line);
}

void Server::swearPolice(Client *c)
{
    std::vector<std::string> targets;
    std::vector<std::string> msgs;
    std::string prefix = ":Kevin!Kevin@localhost";
    msgs.push_back("Your language won't be tolerated for long!");
    msgs.push_back("Demande à ta mère de t'apprendre à parler.");
    targets.push_back(c->getNick());

    size_t i;
    if (c->getWarning() < MAX_WARNINGS)
        i = 0;
    else
        i = msgs.size() - 1;
    c->addWarning();
    c->addReply(RPL_PRIVMSG(prefix, c->getNick(), msgs[i]));
    if (i == msgs.size() - 1)
        c->setClosing();
}

bool Server::checkEquivalentWords(const std::string& word)
{
    static std::map<char, char> equialentChars = {
        {'@', 'a'},
        {'4', 'a'},
        {'8', 'b'},
        {'3', 'e'},
        {'1', 'i'},
        {'!', 'i'},
        {'|', 'i'},
        {'9', 'g'},
        {'1', 'l'},
        {'!', 'l'},
        {'|', 'l'},
        {'0', 'o'},
        {'$', 's'},
        {'5', 's'},
        {'+', 't'},
        {'2', 'z'}
    };
    std::string::const_iterator it = word.begin();
    std::string newWord(word);

    for (; it != word.end(); ++it)
    {
        if (equialentChars.find(*it) != equialentChars.end())
            newWord[it - word.begin()] = equialentChars[*it];
    }
    return _swearWords.find(newWord) != _swearWords.end();
}

static std::string replaceVowels(std::string& str)
{
    std::string vowels = "aeiouy";
    std::string newStr(str);

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (vowels.find(str[i]) != std::string::npos)
            newStr[i] = '*';
    }
    return newStr;
}

std::string Server::censor(std::string& str, int socket)
{
    std::stringstream ss(str);
    std::string word;
    std::string newStr;

    while (ss >> word)
    {
        word = toLower(word);
        if (_swearWords.find(word) != _swearWords.end() || checkEquivalentWords(word))
        {
            word = replaceVowels(word);
            swearPolice(_clients[socket]);
        }
        newStr += word + " ";
    }
    return newStr;
}
