#ifndef UTIL_H
# define UTIL_H

#include <vector>

std::vector<std::string> splitString(const std::string& input, char delimiter);
std::string contcatParams(std::vector<std::string> const& params);

#endif
