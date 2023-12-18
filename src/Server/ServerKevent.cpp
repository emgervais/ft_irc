#include "Server.hpp"
#include "../Client/Client.hpp"

// -- Kevent ----
void Server::initKqueue()
{
    _kq = kqueue();

    if (_kq == -1)
        throw std::runtime_error("Error: kqueue");
    
    const int signalList[] = { SIGINT, SIGQUIT, SIGTERM, SIGSTOP, SIGTSTP, SIGKILL};
    const size_t signalCount = sizeof(signalList) / sizeof(signalList[0]);
    size_t i;
    
    for (i = 0; i < signalCount; ++i)
    {
        EV_SET(_change+i, signalList[i], EVFILT_SIGNAL, EV_ADD, 0, 0, NULL);
        signal(signalList[i], SIG_IGN);
    }
    EV_SET(_change+i, _socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    EV_SET(_change+i+1, fileno(stdin), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(_kq, _change, signalCount+2, NULL, 0, NULL) == ERROR)
        throw std::runtime_error("Error: Kevent: Register server socket to KQueue");
}

void Server::editKevent(int socket, int filter, int flags, std::string msg)
{
    EV_SET(_change, socket, filter, flags, 0, 0, NULL);
    if (kevent(_kq, _change, 1, NULL, 0, NULL) == ERROR)
    {
        bool deleteNonExistent = (errno == ENOENT);
        if (deleteNonExistent)
            return;
        else
            throw std::runtime_error("Error: " + msg);
    }
}

void Server::addWriteKevent()
{
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second && !it->second->getReply().empty())
            editKevent(it->first, EVFILT_WRITE, EV_ADD | EV_ONESHOT, "adding client write to kqueue");
    }
}

int Server::serverQueue()
{
    int evQty = kevent(_kq, NULL, 0, _events, MAX_EVENTS, NULL);
    if (evQty == ERROR)
        throw std::runtime_error("Error: kqueue event creation failed");
    return evQty;
}