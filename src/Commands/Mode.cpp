#include "Command.hpp"
#include "util.hpp"

void    Command::cmdMode()
{
    if (_params.size() == 0)
    {
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
        return;
    }
    Channel *channel = _server.getChannel(_params[0]);
    if (!channel)
    {
        _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[0]));
    }
    else if (_params.size() == 1)
    {
        _client.addReply(RPL_CHANNELMODEIS(_client.getNick(), _client.getMode()));
    }
    std::string mode = _params[0];

}

// void    Command::cmdModeI()
// {
// }

// void    Command::cmdModeO()
// {
// }

// void    Command::cmdModeK()
// {
// }

// void    Command::cmdModeL()
// {
// }

// void    Command::cmdModeT()
// {
// }