#include "Server.hpp"
#include <signal.h>

// -- Singleton --
Server* Server::_instance = 0;

Server* Server::getInstance()
{
    if (_instance == 0)
        throw std::runtime_error("Error: Server instance not created");
    return _instance;
}

// -- Unix signals --
void    handleSigInt(int sig)
{
    (void)sig;
    std::cout << std::endl << "Server is shutting down..." << std::endl;
    Server::getInstance()->~Server();
    exit(0);
}

void    initSignals()
{
    struct sigaction sa;
    sa.sa_handler = handleSigInt;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        throw std::runtime_error("Error: sigaction failed");
}
