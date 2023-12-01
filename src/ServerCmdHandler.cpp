/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmdHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francoma <francoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:17:29 by francoma          #+#    #+#             */
/*   Updated: 2023/12/01 16:30:09 by francoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <vector>
#include "Server.hpp"
#include "NumericReplies.hpp"

static void handleIRCCommand(const std::string& command)
{
    std::cout << "Received IRC command: " << command << std::endl;
}

void Server::cmdHandler(const std::string& data)
{
    std::istringstream iss(data);
    std::string prefix;
    std::string cmd;
    std::string param;
    if (data[0] == ':')
	{
        iss.ignore(1);
        std::getline(iss, prefix, ' ');
    }
    std::getline(iss, cmd, ' ');
    std::getline(iss, param);
    std::cout << "prefix: " << prefix << std::endl
        << "cmd: " << cmd << std::endl
        << "param: " << param << std::endl << std::endl;
	// make a map (CMD, function(prefix, cmd, param))
}

