// #include "Server.hpp"

// int main(int argc, char **argv)
// {
//     try
//     {
//         Server server(argc, argv);
//         server.run();
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << std::endl;
//         return (1);
//     }
//     return (0);
// }

#include "NumericReplies.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

    Server s(argc, argv);
    s.run();
    // std::cout << std::string(ERR_NEEDMOREPARAMS("Emile")) << std::endl;
    // std::cout << std::string(ERR_NEEDMOREPARAMS("Emile")) << std::endl;

}
