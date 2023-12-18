#include "Command.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "NumericReplies.hpp"
#include <sstream>

void    Command::cmdNames()
{
    if (_params.size() == 0)
    {
        _client.addReply(RPL_ENDOFNAMES(_client.getNick(), "*"));
        return;
    }
    std::stringstream ss(_params[0]);
    std::string channelName;
    size_t targets = 0;

    while (std::getline(ss, channelName, ',') && targets < MAX_TARGETS)
    {
        Channel* channel = _server.getChannel(channelName);

        if (channel && (!channel->isMode("s") || channel->isClientOnChannel(_client)))
            _client.addReply(channel->getNamesReply(_client));
        _client.addReply(RPL_ENDOFNAMES(_client.getNick(), channelName));
        targets++;
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
        size_t targets = 0;

        while (std::getline(ss, channelName, ',') && targets < MAX_TARGETS)
        {
            reply = _server.getChannelReply(channelName, _client.getNick());
            if (reply != "")
                _client.addReply(reply);
            targets++;
        }
    }
    _client.addReply(RPL_LISTEND(_client.getNick()));
}
