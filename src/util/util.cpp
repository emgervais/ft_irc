#include "util.hpp"

bool startsWith(std::string const& s1, std::string const& s2)
{
    return !std::strncmp(s1.c_str(), s2.c_str(), s2.size());
}

std::vector<std::string> splitString(const std::string& input, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    size_t found;

    while ((found = input.find(delimiter, pos)) != std::string::npos)
    {
        std::string token = input.substr(pos, found - pos);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
        pos = found + delimiter.length();
    }
    std::string lastToken = input.substr(pos);
    if (!lastToken.empty())
    {
        tokens.push_back(lastToken);
    }
    return tokens;
}

std::string contcatParams(std::vector<std::string> const& params)
{
    std::string res;
    for (size_t i = 0; i < params.size(); ++i)
    {
        res += params[i];
        if (i + 1 != params.size())
            res += " ";
    }
    return (res);
}

// generate a random token of 10 ascii printable characters
std::string randomToken()
{
    srand(time(NULL));
    std::string token;
    for (int i = 0; i < 10; ++i)
        token += rand() % 94 + 33;
    return token;
}

static tm *getUniversalTime()
{
    time_t rawtime;
    tm *timeinfo;

    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    return (timeinfo);
}

std::string getUnixTime()
{
    char buffer[80];

    strftime(buffer, 80, "%s", getUniversalTime());
    return (std::string(buffer));
}

std::string getTimeOfDay()
{
    char buffer[80];

    strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", getUniversalTime());
    return (std::string(buffer));
}

std::string toUpper(const std::string& str)
{
    std::string upper;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        upper += toupper(*it);
    }
    return (upper);
}