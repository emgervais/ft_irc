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

    if (_params.size() > 1)
        nick = contcatParams();
    _client.setNick(nick);
}

void    Command::cmdUser()
{
    if (_params.size() > 4)
        _params[3] = contcatParams(3);
    _client.setUser(_params);
}
