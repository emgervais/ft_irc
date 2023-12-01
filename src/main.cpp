/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francoma <francoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:13:44 by francoma          #+#    #+#             */
/*   Updated: 2023/12/01 16:30:12 by francoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "NumericReplies.hpp"

#include "Server.hpp"



int main(int argc, char *argv[])
{
    Server s(argc, argv);
    s.cmdHandler(":soso PASS fasfa");
    s.cmdHandler("NICK frankoma");
    s.cmdHandler("PRIVMSG :I like turtles");
    // s.run();
    return 0;
}
