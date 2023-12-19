#include "Server.hpp"
#include "../util/util.hpp"
#include "../Commands/Command.hpp"
#include "../Client/Client.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

// -- loop ----
void Server::run()
{
    while (true)
    {
        int nev = serverQueue();
        for (int i = 0; i < nev; ++i)
        {
            if (_events[i].filter == EVFILT_SIGNAL)
                exitSignal(_events[i].ident);
            else if ((int) _events[i].ident == fileno(stdin))
                readFromStdin();
            else if ((int) _events[i].ident == _socket && _clients.size() < MAX_CLIENTS)
                registerNewClient();
            else if (_events[i].filter == EVFILT_READ)
                readFromClient(_events[i].ident);
            else if (_events[i].filter == EVFILT_WRITE)
                writeToClient(_events[i].ident);
        }
        addWriteKevent();
    }
}

// -- receive ----
void Server::registerNewClient()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
    if (clientSocket == ERROR)
    {
        std::cerr << "Error: accepting new client" << std::endl;
        return;
    }
    try
    {
        editKevent(clientSocket, EVFILT_READ, EV_ADD, "adding client read to kqueue");
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Client connection failed. Disconnecting." << std::endl;
        close(clientSocket);
        return;
    }
    Client *newClient = new Client(clientSocket, *this);
    NEW_CONNECTION_MSG(clientSocket);
    _clients.insert(std::make_pair(clientSocket, newClient));
}

void Server::readFromClient(int socket)
{
    if (_clients.find(socket) == _clients.end())
        return;
        
    ssize_t bytesRead = recv(socket, _buffer, sizeof(_buffer) - 1, 0);

    if (bytesRead == ERROR)
        closeClient(socket);
    else if (bytesRead == 0)
        closeClient(socket);
    else
    {
        _buffer[bytesRead] = '\0';
        std::string msg(_buffer);
        if (msg.find("PONG") == std::string::npos)
            FROM_CLIENT(std::string(_buffer));
        handleMsg(socket);
    }
}

void Server::handleMsg(int socket)
{
    std::string input = _clients[socket]->addBuff(_buffer);
    std::vector<std::string> cmds;
    _buffer[0] = '\0';
    if (input.find("\r\n") != std::string::npos)
        cmds = splitString(input, "\r\n");
    else if (input.find("\n") != std::string::npos)
        cmds = splitString(input, "\n");
    else
        return;
    _clients[socket]->chopBuff();
    for (size_t i = 0; i < cmds.size(); ++i)
    {
        Command cmd(*_clients[socket], *this, cmds[i]);
        cmd.exec();
    } 
}

// -- send ----
void Server::writeToClient(int socket)
{
    if (_clients.find(socket) == _clients.end())
        return;
    size_t repliesQty = _clients[socket]->getRepliesQty();
    for (size_t i = 0; i < repliesQty; ++i)
    {
        std::string msg = _clients[socket]->getReply();
        ssize_t bytesSent = send(socket, msg.c_str(), msg.size(), 0);
        if (bytesSent == 0 || bytesSent == ERROR)
        {
            if (bytesSent == ERROR)
                std::cerr << "Error: sending to client" << std::endl;
            closeClient(socket);
            return;
        }
        else
        {
            FROM_SERVER(std::string(msg));
            _clients[socket]->removeReply();
        }
    }
    if (_clients[socket]->isClosing())
        closeClient(socket);
}

// -- read from stdin ----
void Server::readFromStdin()
{
    std::string cmd;
    std::getline(std::cin, cmd);

    if (std::cin.eof())
        exitSignal(SIGINT);

    if (cmd == "/exit" || cmd == "exit")
    {
        std::cerr << "Shutting down server." << std::endl;
        exit(0);
    }
    else if (cmd == "/clients" || cmd == "clients")
    {
        std::map<int, Client*>::iterator it;
        std::cout << "Clients: " << _clients.size() << std::endl;
        for (it = _clients.begin(); it != _clients.end(); ++it)
            std::cout << it->second->getNick() << std::endl;
    }
    else if (cmd == "/channels" || cmd == "channels")
    {
        std::map<std::string, Channel*>::iterator it;
        std::cout << "Channels: " << _channels.size() << std::endl;
        for (it = _channels.begin(); it != _channels.end(); ++it)
            std::cout << it->first << std::endl;
    }
    else if (cmd == "/help" || cmd == "help")
    {
        std::cout << "Available commands:" << std::endl;
        std::cout << "/exit" << std::endl;
        std::cout << "/clients" << std::endl;
        std::cout << "/channels" << std::endl;
        std::cout << "/help" << std::endl;
    }
    else
        std::cout << "Unknown command. Type /help for a list of available commands." << std::endl;
}

// -- exit ----
void Server::exitSignal(int sig)
{
    const char *signal_name = strsignal(sig);

    std::cerr << std::endl << "Quitting due to signal reception: ";
    if (signal_name != nullptr)
        std::cerr << signal_name << " (" << sig << ")" << std::endl;
    else
        std::cerr << "Unknown Signal (" << sig << ")" << std::endl;

    exit(128 + sig);
}
