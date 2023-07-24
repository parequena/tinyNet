#include <iostream>
#include <span> // std::span
#include <vector> // std::vector
#include <ranges> // std::ranges::subrange
#include <cstdint>

#include <Server.hpp>


int better_main(std::span<std::string> const args)
{
    if( args.size() < 1 )
    {
        std::cerr << "Failed with arguments, try executing:\n./executable <port_number>\n";
        return EXIT_FAILURE;
    }
    
    std::uint16_t const port = std::stoi(args[0]) & 0xffff; // std::uint16 max
    tinyNet::Server server{ port };

    while(server.recieveMessages());

    server.pollMessages();

    return EXIT_SUCCESS;
}

int main(int const argc, char const* const* const argv)
{
    auto result = std::ranges::subrange(&argv[1], &argv[argc]);
    std::vector<std::string> args{ result.begin(), result.end() };

// #if defined(TINY_NET_WINDOWS)
//     args.emplace_back("8221");
// #endif
    return better_main(args);
}