#ifndef UTIL_H
# define UTIL_H

#include "CHeaders.hpp"
#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>
#include <queue>

class Client;
class Channel;
class Server;
class Command;

#include "IRC.hpp"
#include "NumericReplies.hpp"
#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"
#include "../Server/Server.hpp"
#include "../Commands/Command.hpp"

bool startsWith(std::string const& s1, std::string const& s2);
std::vector<std::string> splitString(const std::string& input, const std::string& delimiter);
std::string contcatParams(std::vector<std::string> const& params);
std::string randomToken();
std::string getUnixTime();
std::string getTimeOfDay();
std::string toUpper(const std::string& str);

#endif
