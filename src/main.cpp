/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-sage <ele-sage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:13:44 by francoma          #+#    #+#             */
/*   Updated: 2023/12/02 04:57:33 by ele-sage         ###   ########.fr       */
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
    // s.cmdHandler(":soso PASS fasfa");
    // s.cmdHandler("NICK frankofds");
    // s.cmdHandler("PRIVMSG fsddfs: I like turtles\nsfdsdf\nsdfds");
    s.run();
    return 0;
}
