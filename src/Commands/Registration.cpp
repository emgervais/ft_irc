#include "Command.hpp"

void    Command::cmdPass()
{
    std::string pass = _params[0];

    if (_params.size() > 1)
        pass = contcatParams();
    _client.checkPassword(pass);
}

void    Command::cmdNick()
{
    std::string nick = _params[0];

    if (_params.size() != 1)
        nick = contcatParams();
    _client.setNick(nick);
}

void    Command::cmdUser()
{
    if (_params.size() != 4)
        throw std::invalid_argument("Invalid command: wrong number of parameters"); // Need to add a reply for this. Not in IRC RFC
    _client.setUser(_params);
    _reply = RPL_WELCOME(_client.getNick(), _client.getUser(), _client.getHostname());
    _reply += RPL_YOURHOST(_client.getNick());
    _reply += RPL_CREATED(_client.getNick());
    _reply += RPL_MYINFO(_client.getNick());
    _reply += RPL_ISUPPORT(_client.getNick());
}
