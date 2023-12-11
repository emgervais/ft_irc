#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include "Server.hpp"
#include "util.hpp"

struct Args
{
    int port;
    std::string password;
};

static Args getArgs(int argc, char *argv[])
{
    if (argc != 3)
        throw std::invalid_argument("Error: wrong number of arguments");
    Args args;
    std::stringstream ss(argv[1]);
    ss >> args.port;
    if (ss.fail() || !ss.eof() || args.port < 0 || args.port > 65535)
        throw std::invalid_argument("Error: wrong port number");
    ss.str(argv[2]);
    ss.clear();
    ss >> args.password;
    if (ss.fail() || !ss.eof())
        throw std::invalid_argument("Error: wrong password");
    return args;
}

int main(int argc, char *argv[])
{
    try
    {
        Args args = getArgs(argc, argv);
        Server& s = Server::getInstance(args.port, args.password);
        s.run();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl << USAGE << std::endl;
        return 1;
    }    
    catch (std::exception const& e)
    {
        std::cerr << "ça chié dans pelle!" << std::endl
            << e.what() << std::endl;
    }
    return 0;
}
