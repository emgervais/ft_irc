#include "Command.hpp"

const std::string VALID_CMDS[] = {"PASS","NICK","USER","QUIT"}; //,"HELP","PING","PONG","OPER","ERROR","JOIN","PART","TOPIC","NAMES","LIST","INVITE","KICK","MODE","PRIVMSG",};

std::map<const std::string, void (Command::*)()> Command::createCmdHandler()
{
    std::map<const std::string, void (Command::*)()> cmdHandler;

    cmdHandler["PASS"] = &Command::cmdPass;
    cmdHandler["NICK"] = &Command::cmdNick;
    cmdHandler["USER"] = &Command::cmdUser;
    // cmdHandler["PING"] = &Command::cmdPing;
    // cmdHandler["PONG"] = &Command::cmdPong;
    // cmdHandler["OPER"] = &Command::cmdOper;
    cmdHandler["QUIT"] = &Command::cmdQuit;
    // cmdHandler["ERROR"] = &Command::cmdError;
    // cmdHandler["JOIN"] = &Command::cmdJoin;
    // cmdHandler["PART"] = &Command::cmdPart;
    // cmdHandler["TOPIC"] = &Command::cmdTopic;
    // cmdHandler["NAMES"] = &Command::cmdNames;
    // cmdHandler["LIST"] = &Command::cmdList;
    // cmdHandler["INVITE"] = &Command::cmdInvite;
    // cmdHandler["KICK"] = &Command::cmdKick;
    // cmdHandler["MODE"] = &Command::cmdMode;
    // cmdHandler["PRIVMSG"] = &Command::cmdPrivmsg;
    return (cmdHandler);
}

Command::Command(Client &client, Server &server, const std::string raw) : _client(client), _server(server), _raw(raw)
{
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
    if (std::find(std::begin(VALID_CMDS), std::end(VALID_CMDS), _cmd) == std::end(VALID_CMDS))
        throw std::invalid_argument(ERR_UNKNOWNCOMMAND(_client.getNick(), _cmd));
    std::string param;

    while (ss >> param)
    {
        if (param[0] == ':')
        {
            std::string lastParam;
            if (param[1] == ' ')
                lastParam = param.substr(2);
            else
                lastParam = param.substr(1);
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
    std::map<const std::string, void (Command::*)()> cmdHandler = createCmdHandler();

    try
    {
        (this->*cmdHandler[_cmd])();
    }
    catch (std::invalid_argument &e)
    {
        throw std::invalid_argument(ERR_UNKNOWNCOMMAND(_client.getNick(), _cmd));
    }
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