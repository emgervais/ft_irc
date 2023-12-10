#include "Command.hpp"
#include "util.hpp"

void    Command::cmdTopic()
{
    if (_params.size() > 1)
    {
        Channel* channel = _server.getChannel(_params[0]);

        if (channel == NULL)
            _client.addReply(ERR_NOTONCHANNEL(_client.getNick(), _params[0]));
        else if (!channel->isClientOnChannel(_client))
            _client.addReply(ERR_NOTONCHANNEL(_client.getNick(), _params[0]));
        else if (!channel->isMode("t") && !channel->isMode("o", _client.getNick()))
            _client.addReply(ERR_CHANOPRIVSNEEDED(_client.getNick(), _params[0]));
        else
        {
            std::string topic = contcatParams(std::vector<std::string>(_params.begin() + 1, _params.end()));
            if (topic.size() > TOPIC_MAX_LEN)
            {
                topic = topic.substr(0, TOPIC_MAX_LEN - 3);
                topic += "...";
                _client.addReply(ERR_INPUTTOOLONG(_client.getNick()));
            }
            channel->setTopic(topic);
            _client.addReply(RPL_SETTOPIC(_client.getNick(), _client.getUser(), _client.getHostname(),
                                        _params[0], _server.getChannel(_params[0])->getTopic()));
        }
    }
    else if (_params.size() == 1)
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), "TOPIC"));
}

void    Command::cmdNames()
{
    if (_params.size() == 0)
    {
        _client.addReply(RPL_ENDOFNAMES(_client.getNick(), "*"));
        return;
    }
    std::stringstream ss(_params[0]);
    std::string channelName;

    while (std::getline(ss, channelName, ','))
    {
        Channel* channel = _server.getChannel(channelName);

        if (channel)
            _client.addReply(channel->getNamesReply());     
        _client.addReply(RPL_ENDOFNAMES(_client.getNick(), channelName));
    }
}

void    Command::cmdList()
{
    _client.addReply(RPL_LISTSTART(_client.getNick()));
    if (_params.size() == 0)
    {
        std::vector<std::string> channelsList = _server.getChannelsReply(_client.getNick());
        for (size_t i = 0; i < channelsList.size(); i++)
            _client.addReply(channelsList[i]);
    }
    else
    {
        std::stringstream ss(_params[0]);
        std::string channelName;
        std::string reply;

        while (std::getline(ss, channelName, ','))
        {
            reply = _server.getChannelReply(channelName, _client.getNick());
            if (reply != "")
                _client.addReply(reply);
        }
    }
    _client.addReply(RPL_LISTEND(_client.getNick()));
}

void    Command::cmdInvite()
{
    if (_params.size() < 2)
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), "INVITE"));
    else
    {
        Channel* channel = _server.getChannel(_params[1]);

        if (!_server.isNicknameTaken(_params[0]))
            _client.addReply(ERR_NOSUCHNICK(_client.getNick(), _params[0]));
        else if(!channel)
            _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[1]));
        else if (channel->isMode("i") && !channel->isMode("o", _client.getNick()))
            _client.addReply(ERR_CHANOPRIVSNEEDED(_client.getNick(), _params[1]));
        else if (channel->isClientOnChannel(_params[0]))
            _client.addReply(ERR_USERONCHANNEL(_client.getNick(), _params[0], _params[1]));
        else
        {
            Client* client = _server.getClient(_params[0]);
            _client.addReply(RPL_INVITING(_client.getNick(), _params[0], _params[1]));
            client->addReply(RPL_INVITE(_client.getNick(), _params[0], _params[1]));
            if (channel->isMode("i") && channel->isMode("o", _client.getNick()))
                channel->addMode("i", _params[0]);
        }
    }
}

void    Command::cmdKick()
{
    if (_params.size() < 2)
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), "KICK"));
    else
    {
        Channel* channel = _server.getChannel(_params[0]);
        std::string reason = (_params.size() > 2) ? contcatParams(std::vector<std::string>(_params.begin() + 2, _params.end())) : " Stop being a jerk";
        std::stringstream ss(_params[1]);

        if (!channel)
            _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[0]));
        else if (!channel->isClientOnChannel(_client))
            _client.addReply(ERR_NOTONCHANNEL(_client.getNick(), _params[0]));
        else if (!channel->isMode("o", _client.getNick()))
            _client.addReply(ERR_CHANOPRIVSNEEDED(_client.getNick(), _params[0]));
        else
        {
            std::string nick;
            while (std::getline(ss, nick, ','))
            {
                if (!channel->isClientOnChannel(nick))
                    _client.addReply(ERR_USERNOTINCHANNEL(_client.getNick(), nick, _params[0]));
                else
                {
                    Client* client = _server.getClient(nick);
                    channel->removeClient(client, reason);
                }
            }
        }
    }
}