#include "Command.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../util/util.hpp"
#include "NumericReplies.hpp"
#include <sstream>

// -- init ----
std::map<const std::string, cmdFunc> Command::_cmdHandler;
void Command::initCmdHandler()
{
    _cmdHandler["PASS"] = &Command::cmdPass;
    _cmdHandler["NICK"] = &Command::cmdNick;
    _cmdHandler["USER"] = &Command::cmdUser;
    _cmdHandler["PING"] = &Command::cmdPing;
    _cmdHandler["PONG"] = &Command::cmdPong;
    _cmdHandler["QUIT"] = &Command::cmdQuit;
    _cmdHandler["JOIN"] = &Command::cmdJoin;
    _cmdHandler["PART"] = &Command::cmdPart;
    _cmdHandler["TOPIC"] = &Command::cmdTopic;
    _cmdHandler["NAMES"] = &Command::cmdNames;
    _cmdHandler["LIST"] = &Command::cmdList;
    _cmdHandler["INVITE"] = &Command::cmdInvite;
    _cmdHandler["KICK"] = &Command::cmdKick;
    _cmdHandler["MODE"] = &Command::cmdMode;
    _cmdHandler["PRIVMSG"] = &Command::cmdPrivMsg;
    _cmdHandler["HELP"] = &Command::cmdHelp;
}

Command::Command(Client &client, Server &server, std::string const&raw)
    : _client(client), _server(server), _raw(raw)
{
    splitRawCommand();
}

// -- parsing ----
size_t getSizeSpace(std::string const& str)
{
    size_t i;
    for (i = 0; !isspace(str[i]) && str[i] != '\0'; ++i)
    {
    }
    return i;
}

size_t getSizeSep(std::string const& str, std::string const& sep)
{
    size_t i;
    for (i = 0; !startsWith(str.substr(i), sep) && str[i] != '\0'; ++i)
    {
    }
    return i;
}

std::string extractElement(std::string const& _raw, size_t& i)
{
    std::string element = _raw.substr(i);
    size_t size = getSizeSpace(element);
    element = element.substr(0, size);
    i += size + 1;
    return element;
}

std::string extractLastElement(std::string const& _raw, size_t& i)
{
    std::string element = _raw.substr(i);
    size_t size = getSizeSep(element, "\r\n");
    element = element.substr(0, size);
    i += size + 1;
    return element;
}

void    Command::splitRawCommand()
{
    std::stringstream ss(_raw);
    if (_raw.length() > MSG_MAX_LEN)
    {
        _client.addReply(ERR_INPUTTOOLONG(_client.getNick()));
        return;
    }
    if (_raw[0] == ':')
    {
        ss.ignore(1);
        ss >> _prefix;
    }
    ss >> _cmd;
    _cmd = toUpper(_cmd);
    std::string param;
    while (ss >> param)
    {
        if (param[0] == ':')
        {
            std::string lastParam = param.substr(1);
			if (!ss.eof())
            {
                std::getline(ss, param);
                lastParam += param;
            }
            _params.push_back(lastParam);
            break;
        }
        _params.push_back(param);
    }
}

// -- exec ----
void    Command::exec()
{
    cmdFunc f = _cmdHandler[_cmd];
    if (f)
        (this->*f)();
    else
        _client.addReply(ERR_UNKNOWNCOMMAND(_client.getNick(), _cmd));
}

// -- end ----
Command::~Command()
{
}
