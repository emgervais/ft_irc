#include "Command.hpp"

const char * const VALID_CMDS[] = {"PASS","NICK","USER","PING","PONG","OPER","QUIT","ERROR","JOIN","PART","TOPIC","NAMES","LIST","INVITE","KICK","MODE","PRIVMSG",};

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
    std::string::size_type pos = _raw.find_first_of(" \r\n");

    if (pos == std::string::npos)
        throw std::invalid_argument("Invalid command: not terminated by \\r\\n");
    if (pos >= MSG_MAX_LEN)
        throw std::invalid_argument("Invalid command: too long");
    if (_raw[0] == ':')
    {
        ss.ignore(1);
        ss >> _prefix;
    }
    ss >> _cmd;
    if (std::find(std::begin(VALID_CMDS), std::end(VALID_CMDS), _cmd) == std::end(VALID_CMDS))
        throw std::invalid_argument("Invalid command: not a valid command");
    std::string param;
    while (ss >> param)
    {
        if (param[0] == ':')
        {
            param.erase(0, 1);
            param += ss.str().substr(ss.tellg());
            _params.push_back(param);
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
        std::cout << e.what() << std::endl;
    }
}
