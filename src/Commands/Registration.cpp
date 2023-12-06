#include "Command.hpp"
#include "util.hpp"

void    Command::cmdPass()
{
    std::string pass = _params[0];

    if (_params.size() > 1)
        pass = contcatParams(_params);
    _client.checkPassword(pass);
}

void    Command::cmdNick()
{
    std::string nick = _params[0];

    if (_params.size() > 1)
        nick = contcatParams(_params);
    _client.setNick(nick);
}

void    Command::cmdUser()
{
    if (_params.size() > 4)
    {
        std::vector<std::string> lastParams(_params.begin() + 3, _params.end());
        _params[3] = contcatParams(lastParams);
    }
    _client.setUser(_params);
}
