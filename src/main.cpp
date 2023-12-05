/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-sage <ele-sage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:13:44 by francoma          #+#    #+#             */
/*   Updated: 2023/12/04 17:19:40 by ele-sage         ###   ########.fr       */
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
    Server s(argc, argv);
    s.run();
    return 0;
}
