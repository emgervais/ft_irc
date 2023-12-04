#include "Client.hpp"
#include "Server.hpp"

static std::string    getHostName(int socket)
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);
    char                hostname[NI_MAXHOST];

    if (getpeername(socket, (struct sockaddr *)&addr, &len) == -1)
        return ("");
    if (getnameinfo((struct sockaddr *)&addr, len, hostname, sizeof(hostname), NULL, 0, 0) == -1)
        return ("");
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

void            Client::setNick(const std::string& nick)
{
    if (nick.empty())
        throw std::invalid_argument(ERR_NONICKNAMEGIVEN(_nick)); // Need to add a reply for this. Not in IRC RFC
    else if (nick.size() > NICK_MAX_LEN)
        throw std::invalid_argument(ERR_ERRONEUSNICKNAME(nick.substr(0, NICK_MAX_LEN) + "..."));
    else if (_server.isNicknameTaken(nick))
        throw std::invalid_argument(ERR_NICKNAMEINUSE(nick));
    else if (nick.find_first_of(NICK_NOT_CONTAIN) != std::string::npos)
        throw std::invalid_argument(ERR_ERRONEUSNICKNAME(nick));
    else if (nick.find_first_of(NICK_NOT_START) == 0)
        throw std::invalid_argument(ERR_ERRONEUSNICKNAME(nick));
    else if (!isStrPrint(nick))
        throw std::invalid_argument(ERR_ERRONEUSNICKNAME(nick));
    else if (isStrPrint(nick))
        _nick = nick;
    else
        throw std::invalid_argument(ERR_ERRONEUSNICKNAME(nick));
}

// NO documentation on caracter that can be used in username
// Or reply to send if username is invalid
void            Client::setUser(std::vector<std::string> params)
{
    if (isRegistered())
        throw std::invalid_argument(ERR_ALREADYREGISTRED(_nick));
    else if (params.size() < 4)
        throw std::invalid_argument(ERR_NEEDMOREPARAMS(_nick, "USER"));
    else if (params[0].size() > USER_MAX_LEN || params[0].empty())
        throw std::invalid_argument(""); // ERR_ERRONUSERNAME(params[1]); // Need to add a reply for this. Not in IRC RFC
    else if (params[3].size() > REALNAME_MAX_LEN || params[3].empty())
        throw std::invalid_argument(""); // ERR_ERRONUSERNAME(params[3]);
    else if (isStrPrint(params[0]) && isStrPrint(params[3]))
    {
        _user = params[0];
        _realname = params[3];
        _hostname = getHostName(_socket);
        _registered = true;
    }
    else
        throw std::invalid_argument(""); // ERR_ERRONUSERNAME(params[1]);
}

void            Client::checkPassword(std::string password)
{
    static bool     pass = false;

    if (isRegistered() || pass)
        throw std::invalid_argument(ERR_ALREADYREGISTRED(_nick));
    else if (password.empty())
        throw std::invalid_argument(ERR_NEEDMOREPARAMS(_nick, "PASS"));
    else if (password.compare(_server.getPass()) != 0)
        throw std::invalid_argument(ERR_PASSWDMISMATCH(_nick));
    else
        pass = true;
}
