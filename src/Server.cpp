#include <Server.hpp>

#include <netinet/ip.h> // socket, AF_INET6, SOCK_DGRAM, IPPROTO_UDP, IPPROTO_IPV6, IPV6_V6ONLY
#include <iostream> // std::cout
#include <unistd.h> // close
#include <string.h> // strerror
#include <string> // std::string

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Server::Server(std::uint16_t port)
    : socketFd_{ socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP) }
    , port_ { port }
{
    std::cout << "Server listening port: " << port_ << '\n';
    checkValue(socketFd_, "Failed creating socket() : " + std::string{ strerror(errno) } + ".\n");

    sockaddr_in6 serverInfo{};
    serverInfo.sin6_family = AF_INET6;
    serverInfo.sin6_addr = in6addr_any;
    serverInfo.sin6_port = htons(port_);

    checkValue(bind(socketFd_, reinterpret_cast<sockaddr const*>(&serverInfo), sizeof(serverInfo)),
        "Failed bind() : " + std::string{ strerror(errno) } + ".\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Dtor.
Server::~Server() noexcept
{
    std::cout << "Server destruction!\n";
    close(socketFd_);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recieve Messages and print them.
bool Server::recieveMessages() noexcept
{
    std::string buffer{};
    buffer.resize(1024);

    sockaddr_in6 clientInfo{};
    socklen_t size { sizeof(clientInfo) };

    auto const recfrom = recvfrom(socketFd_, buffer.data(), buffer.size(), MSG_WAITALL, reinterpret_cast<sockaddr*>(&clientInfo), &size);
    if(recfrom == -1)
    {
        std::cerr << "Failed recvfrom() " << strerror(errno) << "\n";
        return false;
    }

    std::string const recieved { buffer.begin(), buffer.begin() + recfrom };
    if(recieved == "quit")
    {
        return false;
    }

    messages_.emplace( recieved );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Print Messages.
void Server::pollMessages() noexcept
{
    std::cout << "Messages:\n";
    while(!messages_.empty())
    {
        auto const& front = messages_.front();
        std::cout << "[\t" + front.data + "\t]\n";
        messages_.pop();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check Value.
void Server::checkValue(int const value, std::string const& message)
{
    if(value == -1)
    {
        throw std::invalid_argument( message ); 
    }
}

} // namespace tinyNet