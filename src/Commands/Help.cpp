#include "Command.hpp"
#include "NumericReplies.hpp"
#include "../Client/Client.hpp"

static void helpChannelModes(Client *client)
{
    client->addReply(RPL_HELPSTART(client->getNick(), "** Channel Modes **"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_HELPTXT(client->getNick(), "i - Invite only"));
    client->addReply(RPL_HELPTXT(client->getNick(), "k - Channel key"));
    client->addReply(RPL_HELPTXT(client->getNick(), "l - Channel limit"));
    client->addReply(RPL_HELPTXT(client->getNick(), "n - No external messages"));
    client->addReply(RPL_HELPTXT(client->getNick(), "o - Give/take channel operator privilege"));
    client->addReply(RPL_HELPTXT(client->getNick(), "t - Topic settable by channel operator only"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_ENDOFHELP(client->getNick()));
}

static void helpUser(Client *client)
{
    client->addReply(RPL_HELPSTART(client->getNick(), "** User Commands **"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_HELPTXT(client->getNick(), "JOIN     -      To join a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "PART     -      To leave a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "NAMES    -      To list all users in a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "LIST     -      To list all channels"));
    client->addReply(RPL_HELPTXT(client->getNick(), "INVITE   -      To invite a user to a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "KICK     -      To kick a user from a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "PRIVMSG  -      To send a message to a user or a channel"));
    client->addReply(RPL_HELPTXT(client->getNick(), "MODE     -      To change a channel mode"));
    client->addReply(RPL_HELPTXT(client->getNick(), "TOPIC    -      To change a channel topic"));
    client->addReply(RPL_HELPTXT(client->getNick(), "NICK     -      To change your nickname"));
    client->addReply(RPL_HELPTXT(client->getNick(), "QUIT     -      To quit the server"));
    client->addReply(RPL_HELPTXT(client->getNick(), "HELP     -      To get help"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_ENDOFHELP(client->getNick()));
}

static void help(Client *client)
{
    client->addReply(RPL_HELPSTART(client->getNick(), "** Help System **"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_HELPTXT(client->getNick(), "This system provides help for commands and modes."));
    client->addReply(RPL_HELPTXT(client->getNick(), "Specify one of the following parameters for more information:"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_HELPTXT(client->getNick(), "USERCMD    -      To see a list of user commands"));
    client->addReply(RPL_HELPTXT(client->getNick(), "CHANMODES  -      To see a list of channel modes"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_HELPTXT(client->getNick(), "Example:   -      HELP USERCMD"));
    client->addReply(RPL_HELPTXT(client->getNick(), ""));
    client->addReply(RPL_ENDOFHELP(client->getNick()));
}

void    Command::cmdHelp()
{
    if (_params.size() == 0)
        help(&_client);
    else if (_params[0] == "USERCMD")
        helpUser(&_client);
    else if (_params[0] == "CHANMODES")
        helpChannelModes(&_client);
    else
        _client.addReply(ERR_HELPNOTFOUND(_client.getNick(), _params[0]));
}
