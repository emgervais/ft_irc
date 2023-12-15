#include "Server.hpp"
#include "../util/util.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "NumericReplies.hpp"
#include <fstream>

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
        if (it->first == name && (it->second->isClientOnChannel(clientNick) || it->second->isMode("n") == false))
            return (RPL_LIST(clientNick, it->second->getName(), it->second->getUsersCount(), it->second->getTopic()));
    }
    return "";
}

std::vector<std::string> Server::getChannelsReply(const std::string& clientNick) const
{
    std::vector<std::string> channelsList;
    std::map<std::string, Channel*>::const_iterator it;

    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second->isClientOnChannel(clientNick) || it->second->isMode("n") == false)
            channelsList.push_back(RPL_LIST(clientNick, it->second->getName(), it->second->getUsersCount(), it->second->getTopic()));
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
    if (!file.is_open())
    {
        std::cerr << "Error opening SWEAR_WORDS_PATH: " << SWEAR_WORDS_PATH << std::endl;
        return;
    }

    std::string swearWord;
    while (std::getline(file, swearWord))
    {
        std::transform(swearWord.begin(), swearWord.end(), swearWord.begin(), ::tolower);
        swearWordsSet.insert(swearWord);
    }
    file.close();
}

void Server::swearPolice(Client *c) {
    c->addWarning();
    if(c->getWarning() >= 3) {
        c->addReply("BadWordsPolice : Demande à ta mère de t'apprendre à parler");
        c->partAllChannels();
        c->setClosing();
    }
    else {
        c->addReply("BadWordsPolice : Your language wont be tolerated for long!");
    }
}

void Server::censor(std::string& str, Client* c)
{
    bool Warned = false;
    if (swearWordsSet.empty())
        loadSwearWords();
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    std::set<std::string>::iterator it;
    for (it = swearWordsSet.begin(); it != swearWordsSet.end(); ++it)
    {
        std::string swearWord = *it;
        size_t found = lowerStr.find(swearWord);
        while (found != std::string::npos)
        {
            Warned = true;
            size_t vowel = str.find_first_of("aeiouyAEIOUY", found);
            if (vowel != std::string::npos)
                str[vowel] = '*';
            found = lowerStr.find(swearWord, found + 1);
        }
    }
    if(Warned)
        swearPolice(c);
}
