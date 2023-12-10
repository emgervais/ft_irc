#ifndef UTIL_H
# define UTIL_H

#include <vector>

bool startsWith(std::string const& s1, std::string const& s2);
std::vector<std::string> splitString(const std::string& input, const std::string& delimiter);
std::string contcatParams(std::vector<std::string> const& params);
std::string randomToken();
std::string getTimeOfDay()

#endif
