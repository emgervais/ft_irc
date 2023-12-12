#include "Command.hpp"

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
        _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), _params[1]));
        // if (channel->isMode("o", _client.getNick()) == false)
        // {
        //     _client.addReply(ERR_CHANOPRIVSNEEDED(_client.getNick(), channel->getName()));
        //     return;
        // }
        // std::string::iterator it = _params[1].begin();
        // std::string mode = _params[1];
        // std::vector<std::string>::iterator params;
        // std::string param = "";
        // char sign = '+';

        // if (_params.size() > 2)
        //     params = _params.begin() + 2;
        // while (it != _params[1].end())
        // {
        //     if (*it == 'o'|| *it == 'k' || *it == 'l')
        //     {
        //         if (params == _params.end())
        //             param = "";
        //         else
        //             param = *params++;
        //     }
        //     if (*it == '+' || *it == '-')
        //         sign = *it;
        //     else if (*it == 'i')
        //         cmdModeI(channel, sign);
        //     else if (*it == 'o')
        //         cmdModeO(channel, sign, param);
        //     else if (*it == 'k')
        //         cmdModeK(channel, sign, param);
        //     else if (*it == 'l')
        //         cmdModeL(channel, sign, param);
        //     else if (*it == 't')
        //         cmdModeT(channel, sign);
        //     else
        //         _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), *it));
        //     ++it;
        // }
    }
}

// void    Command::cmdModeO(Channel *channel, char sign, const std::string& param)
// {
//     if (param.empty())
//         _client.addReply(ERR_SPECIFYOP(_client.getNick()));
//     else
//     {
//         if (sign == '+')
//         {
//             if (!channel->isMode("o", param))
//                 channel->addMode("o", param);
//         }
//         else if (sign == '-')
//         {
//             if (channel->isMode("o", param))
//                 channel->removeMode("o", param);
//         }
//     }
// }

// void    Command::cmdModeI(Channel *channel, char sign)
// {

// }

// void    Command::cmdModeK(Channel *channel, char sign, const std::string& param)
// {
//     if (param.empty())
//         _client.addReply(ERR_SPECIFYKEY(_client.getNick()));
//     else
//     {
//         if (sign == '+')
//         {
//             if (!channel->isMode("k", param))
//             {
//                 // check if key is correct
//                 channel->addMode("k", param);
//             }
//         }
//         else if (sign == '-')
//         {
//             if (channel->isMode("k", param))
//                 channel->removeMode("k", param);
//         }
//     }
// }

// void    Command::cmdModeL(Channel *channel, char sign, const std::string& param)
// {

// }

// void    Command::cmdModeT(Channel *channel, char sign)
// {

// }
