#include "Client.hpp"
#include "../Server/Server.hpp"
#include "../util/util.hpp"
#include "NumericReplies.hpp"

Client::Client(int socket, Server &server)
    : _socket(socket), _nick(""), _user(""), _realname(""), _hostname(""), _ping(randomToken()), _waitingForPong(false), _server(server), _registered(false), _passChecked(false), _closing(false), _warnings(0), _mode("")
{
}

Client::~Client()
{
}

std::string     Client::getPrefix() const
{
    return (":" + _nick + "!" + _user + "@" + _hostname);
}

std::string     Client::getNick() const
{
    return (_nick);
}

std::string     Client::getUser() const
{
    return (_user);
}

std::string     Client::getRealname() const
{
    return (_realname);
}

std::string     Client::getHostname() const
{
    return (_hostname);
}

int             Client::getSocket() const
{
    return (_socket);
}

std::string     Client::getPing() const
{
    return (_ping);
}

bool            Client::isRegistered() const
{
    return (_registered);
}

void            Client::setRegistered()
{
    _registered = true;
}

bool            Client::isWaitingForPong() const
{
    return (_waitingForPong);
}

void            Client::disableWaitingForPong()
{
    _waitingForPong = false;
}

void            Client::addReply(std::string const& reply)
{
    if (!_closing)
        _sendQueue.push(reply);
}

void            Client::removeReply()
{
    _sendQueue.pop();
}

std::string  Client::getReply() const
{
    if (_sendQueue.empty())
    {
        return ("");
    }
    return _sendQueue.front();
}

size_t Client::getRepliesQty() const
{
    return _sendQueue.size();
}

void            Client::setClosing()
{
    _closing = true;
}

bool            Client::isClosing() const
{
    return (_closing);
}

void            Client::addWarning() {
    _warnings++;
}

int             Client::getWarning() {
    return _warnings;
}

void            Client::setMode(const std::string& mode)
{
    char sign = '+';
    std::string modeChange = "";
    std::string::const_iterator it = mode.begin();

    while (it != mode.end())
    {
        if (*it == '+' || *it == '-')
            sign = *it;
        else if (*it == 'i' && ((sign == '+' && _mode.empty()) || (sign == '-' && _mode == "+i")))
            modeChange = std::string(1, sign) + std::string(1, *it);
        else
            addReply(ERR_UMODEUNKNOWNFLAG(_nick, *it));
        ++it;
    }
    if (mode.find('i') != std::string::npos)
    {
        addReply(RPL_UMODEIS(_nick, modeChange));
        if (modeChange == "+i")
            _mode = modeChange;
        else
            _mode = "";
    }
}

bool            Client::isInvisible() const
{
    return (_mode == "+i");
}

std::string     Client::getMode() const
{
    return (_mode);
}