#include "Command.hpp"

void Command::initCmdHandler()
{
    _cmdHandler["PASS"] = &Command::cmdPass;
    _cmdHandler["NICK"] = &Command::cmdNick;
    _cmdHandler["USER"] = &Command::cmdUser;
    // _cmdHandler["PING"] = &Command::cmdPing;
    // _cmdHandler["PONG"] = &Command::cmdPong;
    // _cmdHandler["OPER"] = &Command::cmdOper;
    _cmdHandler["QUIT"] = &Command::cmdQuit;
    // _cmdHandler["ERROR"] = &Command::cmdError;
    // _cmdHandler["JOIN"] = &Command::cmdJoin;
    // _cmdHandler["PART"] = &Command::cmdPart;
    // _cmdHandler["TOPIC"] = &Command::cmdTopic;
    // _cmdHandler["NAMES"] = &Command::cmdNames;
    // _cmdHandler["LIST"] = &Command::cmdList;
    // _cmdHandler["INVITE"] = &Command::cmdInvite;
    // _cmdHandler["KICK"] = &Command::cmdKick;
    // _cmdHandler["MODE"] = &Command::cmdMode;
    // _cmdHandler["PRIVMSG"] = &Command::cmdPrivmsg;
}

Command::Command(Client &client, Server &server, const std::string raw) : _client(client), _server(server), _raw(raw)
{
    initCmdHandler();
    splitRawCommand();
}

Command::~Command()
{
}

void    Command::splitRawCommand()
{
    std::stringstream ss(_raw);
    if (_raw.length() > MSG_MAX_LEN)
        throw std::invalid_argument(ERR_INPUTTOOLONG(_client.getNick()));
    if (_raw[0] == ':')
    {
        ss.ignore(1);
        ss >> _prefix;
    }
    ss >> _cmd;
   
    std::string param;
    while (ss >> param)
    {
        if (param[0] == ':')
        {
            std::string lastParam = param.substr(1);
            std::getline(ss, param);
            lastParam += param;
            _params.push_back(lastParam);
            break;
        }
        _params.push_back(param);
    }
}

void    Command::exec()
{
    // (this->*_cmdHandler[_cmd])();

    cmdFunc f = _cmdHandler[_cmd];
    if (f)
        (this->*f)();
    else
        throw std::invalid_argument(ERR_UNKNOWNCOMMAND(_client.getNick(), _cmd));
}

std::string Command::getReply() const
{
    return (_reply);
}

std::string Command::contcatParams() const
{
    std::string params;

    for (std::vector<std::string>::const_iterator it = _params.begin(); it != _params.end(); ++it)
    {
        params += *it;
        if (it + 1 != _params.end())
            params += " ";
    }
    return (params);
}
