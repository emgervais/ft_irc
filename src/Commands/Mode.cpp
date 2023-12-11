#include "Command.hpp"
#include "util.hpp"
#include "NumericReplies.hpp"

void    Command::cmdMode()
{
    if (_params.size() == 0)
    {
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
        return;
    }
    Channel *channel = _server.getChannel(_params[0]);
    if (!channel)
        _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[0]));
    else if (_params.size() == 1)
        _client.addReply(RPL_CHANNELMODEIS(_client.getNick(), channel->getName(), channel->getModeString()));
    else
    {
        std::string mode = _params[1];
        if (mode[0] != '+' && mode[0] != '-' && mode[0] != 'i' && mode[0] != 'o' && mode[0] != 'k' && mode[0] != 'l' && mode[0] != 't')
            _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), mode[0]));
        else
        {
            if (mode[0] == '+' || mode[0] == '-')
                mode = mode.substr(1);
            // if (mode[0] == 'i')
            //     cmdModeI(channel);
            // else if (mode[0] == 'o')
            //     cmdModeO(channel);
            // else if (mode[0] == 'k')
            //     cmdModeK(channel);
            // else if (mode[0] == 'l')
            //     cmdModeL(channel);
            // else if (mode[0] == 't')
            //     cmdModeT(channel);
            // else
             _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), mode[0]));
        }
    }
}

// t = topic takes no parameter
// k = key takes parameter if +, no parameter if -
// l = limit takes parameter if +, no parameter if -
// i = invite only takes no parameter
// o = operator always takes parameter

// void    Command::cmdModeI(Channel *channel)
// {
// }

// void    Command::cmdModeO(Channel *channel)
// {
// }

// void    Command::cmdModeK(Channel *channel)
// {
// }

// void    Command::cmdModeL(Channel *channel)
// {
// }

// void    Command::cmdModeT(Channel *channel)
// {
// }