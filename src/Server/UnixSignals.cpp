#include "Server.hpp"
#include <signal.h>

// -- Singleton --
Server* Server::_instance = NULL;

Server* Server::getInstance()
{
    if (_instance == NULL)
        throw std::runtime_error("Error: Server instance not created");
    return _instance;
}

// -- Unix signals --
static void    handleSignal(int sig)
{
    (void)sig;
    std::cout << std::endl << "Server is shutting down..." << std::endl;
    Server::getInstance()->~Server();
    exit(0);
}

void    initSignals()
{
    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1
        || sigaction(SIGTERM, &sa, NULL) == -1
        || sigaction(SIGHUP, &sa, NULL) == -1
        || sigaction(SIGQUIT, &sa, NULL) == -1
        // || sigaction(SIGUSR1, &sa, NULL) == -1
        // || sigaction(SIGUSR2, &sa, NULL) == -1)
    )
        throw std::runtime_error("Error: sigaction failed");
}
