/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francoma <francoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:13:44 by francoma          #+#    #+#             */
/*   Updated: 2023/12/05 14:11:13 by francoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "Server.hpp"

int main(int argc, char *argv[])
{
    //std::cout << "Process ID (PID): " << getpid() << std::endl;
    try
    {
        Server s(argc, argv);
        s.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "ça chié dans pelle!" << std::endl
            << e.what() << std::endl;
    }
    return 0;
}
