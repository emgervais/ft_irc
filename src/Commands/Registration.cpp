#include "Command.hpp"
#include "../util/util.hpp"
#include "../Client/Client.hpp"

void    Command::cmdPass()
{
    _client.checkPassword(contcatParams(_params));
}

void    Command::cmdNick()
{
    _client.setNick(contcatParams(_params));
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
