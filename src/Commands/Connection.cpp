#include "Command.hpp"

// TODO:
// cmdJoin
// cmdPart
// cmdPing
// cmdPong

void    Command::cmdQuit()
{
    if (_params.size() > 1)
        _reply = RPL_QUIT(_client.getUser(), _client.getHostname(), contcatParams());
    else
        _reply = RPL_QUIT(_client.getUser(), _client.getHostname(), _params[0]);
}
