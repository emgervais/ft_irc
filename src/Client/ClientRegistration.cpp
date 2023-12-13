#include "Client.hpp"

static std::string    getHostName(int socket)
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);
    char                hostname[NI_MAXHOST];

    if (getpeername(socket, (struct sockaddr *)&addr, &len) == -1)
        return ("Unknown");
    if (getnameinfo((struct sockaddr *)&addr, len, hostname, sizeof(hostname), NULL, 0, 0) == -1)
        return ("Unknown");
    return (std::string(hostname));
}

static bool isStrPrint(const std::string& str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!isprint(*it) && *it != '\r' && *it != '\n')
        {
            std::cout << "Char not printable: " << *it << " (" << (int)*it << ")" << std::endl;
            return (false);
        }
    }
    return (true);
}

void    Client::setNick(const std::string& nick)
{
    std::string oldNick = "NICK";

    if (!_nick.empty())
        oldNick = _nick;
    if (!_passChecked)
        addReply(ERR_PASSWREQ(oldNick));
    else if (nick.empty())
        addReply(ERR_NONICKNAMEGIVEN(oldNick));
    else if (nick.size() > NICK_MAX_LEN)
        addReply(ERR_ERRONEUSNICKNAME(nick.substr(0, NICK_MAX_LEN) + "..."));
    else if (_server.isNicknameTaken(nick))
        addReply(ERR_NICKNAMEINUSE(nick));
    else if (nick.find_first_of(NICK_NOT_CONTAIN) != std::string::npos)
        addReply(ERR_ERRONEUSNICKNAME(nick));
    else if (nick.find_first_of(NICK_NOT_START) == 0)
        addReply(ERR_ERRONEUSNICKNAME(nick));
    else if (!isStrPrint(nick))
        addReply(ERR_ERRONEUSNICKNAME(nick));
    else if (isStrPrint(nick))
        _nick = nick;

}

void    Client::setUser(std::vector<std::string> params)
{
    std::string oldNick = "NICK";

    if (!_nick.empty())
        oldNick = _nick;
    if (!_passChecked)
        addReply(ERR_PASSWREQ(oldNick));
    else if (isRegistered())
        addReply(ERR_ALREADYREGISTRED(_nick));
    else if (params.size() < 4)
        addReply(ERR_NEEDMOREPARAMS(oldNick, "USER"));
    else if (params[0].size() > USER_MAX_LEN || params[0].empty())
        addReply(ERR_ERRONUSERNAME(oldNick));
    else if (params[3].size() > REALNAME_MAX_LEN || params[3].empty())
        addReply(ERR_ERRONUSERNAME(oldNick));
    else if (isStrPrint(params[0]) && isStrPrint(params[3]))
    {
        _user = params[0];
        _realname = params[3];
        _hostname = getHostName(_socket);
        _waitingForPong = true;
        addReply("PING :" + _ping + CRLF);
    }
}

void    Client::checkPassword(std::string password)
{
    if (isRegistered() || _passChecked)
        addReply(ERR_ALREADYREGISTRED(_nick));
    else if (password.empty())
        addReply(ERR_NEEDMOREPARAMS("NICK", "PASS"));
    else if (password.compare(_server.getPass()) != 0)
        addReply(ERR_PASSWDMISMATCH("NICK"));
    else
        _passChecked = true;
}
