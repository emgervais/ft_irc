/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCmdHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-sage <ele-sage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:17:29 by francoma          #+#    #+#             */
/*   Updated: 2023/12/02 02:44:16 by ele-sage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <vector>
#include "Server.hpp"
#include "NumericReplies.hpp"

// static void handleIRCCommand(const std::string& command)
// {
//     std::cout << "Received IRC command: " << command << std::endl;
// }

void Server::handleMsg(const std::string& data)
{
    std::istringstream iss(data);
    std::string prefix;
    std::string cmd;
    std::string param;
    if (data[0] == ':')
	{
        iss.ignore(1);
		iss >> prefix;
    }
	iss >> cmd;
	std::getline(iss, param, '\0'); 
	
    std::cout << "prefix: " << prefix << std::endl
        << "cmd: " << cmd << std::endl
        << "param: " << param << std::endl << std::endl;
}

