#include "Command.hpp"

void    Command::cmdPass()
{
    if (_params.size() != 1)
        throw std::invalid_argument("Invalid command: wrong number of parameters"); // Need to add a reply for this. Not in IRC RFC
    if (!_client.checkPassword(_params[0]))
        throw std::invalid_argument(_client.getReply());
}

void    Command::cmdNick()
{
    if (_params.size() != 1)
        throw std::invalid_argument("Invalid command: wrong number of parameters"); // Need to add a reply for this. Not in IRC RFC
    if (!_client.setNick(_params[0]))
        throw std::invalid_argument(_client.getReply());
}

void    Command::cmdUser()
{
    if (_params.size() != 4)
        throw std::invalid_argument("Invalid command: wrong number of parameters"); // Need to add a reply for this. Not in IRC RFC
    if (!_client.setUser(_params))
        throw std::invalid_argument(_client.getReply());
    else
        _client.setNewlyRegistered(true);
}
