#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
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
