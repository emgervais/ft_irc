#include "Client.hpp"
#include "Server.hpp"

static bool isStrPrint(const std::string& str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!isprint(*it))
            return (false);
    }
    return (true);
}

bool    Client::isNickValid(const std::string& nick)
{   
    if (_registrationStep != 1)
        return (false);
    else if (nick.empty())
        _reply = ERR_NONICKNAMEGIVEN(_nick);
    else if (nick.size() > NICK_MAX_LEN)
        _reply = ERR_ERRONEUSNICKNAME(nick.substr(0, NICK_MAX_LEN) + "...");
    else if (!_server.isNicknameTaken(nick))
        _reply = ERR_NICKNAMEINUSE(nick);
    else if (nick.find_first_of(NICK_NOT_CONTAIN) != std::string::npos)
        _reply = ERR_ERRONEUSNICKNAME(nick);
    else if (nick.find_first_of(NICK_NOT_START) == 0)
        _reply = ERR_ERRONEUSNICKNAME(nick);
    else if (!isStrPrint(nick))
        _reply = ERR_ERRONEUSNICKNAME(nick);
    else
        return (true);
    return (false);
}

// NO documentation on caracter that can be used in username
// Or reply to send if username is invalid
bool    Client::isUserValid(const std::vector<std::string>& params)
{
    if (isRegistered())
        _reply = ERR_ALREADYREGISTRED(_nick);
    else if (params.size() < 4)
        _reply = ERR_NEEDMOREPARAMS(_nick, "USER");
    else if (params[1].size() > USER_MAX_LEN || params[1].empty())
        _reply = ""; // ERR_ERRONUSERNAME(params[1]); // Need to add a reply for this. Not in IRC RFC
    else if (params[2].size() > USER_MAX_LEN || params[2].empty())
        _reply = ""; // ERR_ERRONUSERNAME(params[2]);
    else if (params[3].size() > USER_MAX_LEN || params[3].empty())
        _reply = ""; // ERR_ERRONUSERNAME(params[3]);
    else if (isStrPrint(params[1]) && isStrPrint(params[2]) && isStrPrint(params[3]))
        return (true);
    return (false);
}

bool    Client::isPassValid(const std::string& password)
{
    if (isRegistered())
        _reply = ERR_ALREADYREGISTRED(_nick);
    else if (password.empty())
        _reply = ERR_NEEDMOREPARAMS(_nick, "PASS");
    else if (password.compare(_server.getPass()) != 0)
        _reply = ERR_PASSWDMISMATCH(_nick);
    else
        return (true);
    return (false);
}