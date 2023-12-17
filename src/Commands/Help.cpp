#include "Command.hpp"
#include "NumericReplies.hpp"
#include "../Client/Client.hpp"
#include "../util/util.hpp"

static void helpJoin(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "JOIN <channel>[,<channel>]+ [<key>[,<key>]+]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Joins a channel. If the channel does not exist, it will be created."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "If a key is specified, it will be used to join the channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "JOIN #test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "JOIN #test key"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "JOIN #test1,#test2 key1,key2"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpPart(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "PART <channel>[,<channel>]+ [:<reason>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Leaves a channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "If a message is specified, it will be sent to the channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PART #test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PART #test :bye"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PART #test1,#test2 :bye"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpNames(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "NAMES [<channel>[,<channel>]+]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Lists all users in a specified channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, command));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "NAMES #test1,#test2"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpList(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "LIST [<channel>{,<channel>}+]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Lists all channels."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, command));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "LIST #test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "LIST #test1,#test2"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpInvite(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "INVITE <nickname> <channel>"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Invites a user to a channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "INVITE test #test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpKick(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "KICK <channel> <nick>[,<nick>]+ [:<reason>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Kicks a user from a channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "KICK #test test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "KICK #test test1,test2 :bye"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpPrivmsg(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "PRIVMSG <target>[,<target>]+ :<message>"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Sends a message to a user or a channel."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PRIVMSG test :hello"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PRIVMSG test1,#test2 :hello"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpMode(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "MODE <channel> <mode>[,<mode>]+ [<mode arguments>[,<mode arguments>]+]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Changes a channel mode."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "MODE #test +i"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "MODE #test +o test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "MODE #test +l 10"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "MODE #test +k-ol key test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpTopic(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "TOPIC <channel> [:<topic>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Changes a channel topic."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "TOPIC #test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "TOPIC #test :hello"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpNiclk(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "NICK <nickname>"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Changes your nickname."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "NICK test"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpPass(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "PASS <password>"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "This command is used by your client when setting up"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "your IRC session to submit a server password to the server."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "You should not use it during an established connection."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpPing(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "PING <cookie> [<servername>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "<cookie> is a random string of characters."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "If <servername> is specified, the PING will be forwarded to it."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Otherwise, the PING will be sent to all servers."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "<servername> is the name of the server to which the PING is being forwarded."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "If <servername> is not specified, the PING is being sent to all servers."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Examples:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PING 123456"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PING 123456 server.localhost"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpPong(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "PONG <cookie> [<servername>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Your client should send this command to the server in response to a PING."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "should not issue this command manually."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpQuit(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "QUIT [:<reason>]"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Ends your IRC session."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Example:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "QUIT :bye"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpUser(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "USER <username> <unused> <unused> :<realname>"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "This command is used by your client to register your"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "IRC session, providing your ident and real name to the server."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "You should not use it during an established connection."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpChannelModes(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "Channel Modes"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "i - Invite only"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "k - Channel key"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "l - Channel limit"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "n - No external messages"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "o - Give/take channel operator privilege"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "t - Topic settable by channel operator only"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void helpUserCmd(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "User Commands"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "JOIN     -      To join a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PART     -      To leave a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "NAMES    -      To list all users in a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "LIST     -      To list all channels"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "INVITE   -      To invite a user to a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "KICK     -      To kick a user from a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PRIVMSG  -      To send a message to a user or a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "MODE     -      To change a channel mode"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "TOPIC    -      To change a channel topic"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "NICK     -      To change your nickname"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PASS     -      To set a password"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PING     -      To ping the server"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "PONG     -      To pong the server"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "QUIT     -      To quit the server"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "USER     -      To set your username"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

static void help(Client *client, const std::string &command)
{
    client->addReply(RPL_HELPSTART(client->getNick(), command, "Help System"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "This system provides help for commands and modes."));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Specify one of the following parameters for more information:"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "USERCMD    -      To see a list of user commands"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "CHANMODES  -      To see a list of channel modes"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_HELPTXT(client->getNick(), command, "Example:   -      HELP USERCMD"));
    client->addReply(RPL_HELPTXT(client->getNick(), command, ""));
    client->addReply(RPL_ENDOFHELP(client->getNick(), command));
}

struct s_help
{
    std::string command;
    void (*help)(Client *client, const std::string &command);
};

void    Command::cmdHelp()
{
    std::string topic = "HELP";
    struct s_help helps[] = {
        {"HELP", help},
        {"JOIN", helpJoin},
        {"PART", helpPart},
        {"NAMES", helpNames},
        {"LIST", helpList},
        {"INVITE", helpInvite},
        {"KICK", helpKick},
        {"PRIVMSG", helpPrivmsg},
        {"MODE", helpMode},
        {"TOPIC", helpTopic},
        {"NICK", helpNiclk},
        {"PASS", helpPass},
        {"PING", helpPing},
        {"PONG", helpPong},
        {"QUIT", helpQuit},
        {"USER", helpUser},
        {"USERCMD", helpUserCmd},
        {"CHANMODES", helpChannelModes},
    };

    if (_params.size() > 0)
        topic = toUpper(_params[0]);
    for (size_t i = 0; i < sizeof(helps) / sizeof(struct s_help); ++i)
    {
        if (topic == helps[i].command)
        {
            helps[i].help(&_client, _cmd);
            return;
        }
    }
    _client.addReply(ERR_HELPNOTFOUND(_client.getNick(), _params[0]));
}
