/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-sage <ele-sage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:13:44 by francoma          #+#    #+#             */
/*   Updated: 2023/12/01 16:30:23 by ele-sage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "NumericReplies.hpp"

#include "Server.hpp"


void handleIRCCommand(const std::string& command)
{
    std::cout << "Received IRC command: " << command << std::endl;
}


std::string const& extractCmd(std::vector<std::string> const& tokens)
{
    if (tokens.size() < 2)
        return "";
    if (tokens[0][0] == ':')
        return tokens[2];
    return tokens[1];
}

std::string const& extractParam(std::vector<std::string> const& tokens)
{
    if (tokens.size() < 2)
        return "";
    if (tokens[0][0] == ':')
        return tokens[1];
    return tokens[0];    
}

void parseAndHandleCommands(const std::string& data) {
    std::istringstream iss(data);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    std::string const& cmd = extractCmd(tokens);

    // for (size_t i = 0; i < tokens.size(); ++i)
        // std::cout << tokens[i] << std::endl;
        
    size_t cmdCount = sizeof(CMDS) / sizeof(CMDS[0]);
    for (size_t i = 0; i < cmdCount; ++i)
    {
        if (cmd == CMDS[i])
        {
            handleIRCCommand(cmd);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
//     parseAndHandleCommands(":soso PASS fasfa");
    Server s(argc, argv);
    s.run();
    return 0;
}
