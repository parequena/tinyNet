#include <Server.hpp>

#if defined(TINY_NET_LINUX)
#include <netinet/ip.h> // socket, AF_INET6, SOCK_DGRAM, IPPROTO_UDP, IPPROTO_IPV6, IPV6_V6ONLY
#include <unistd.h> // close
#include <string.h> // strerror
#else
#include <ws2ipdef.h>
#include <ws2tcpip.h>

// Link with sockets lib.
#pragma comment(lib,"Ws2_32.lib")
#endif // defined(TINY_NET_LINUX)

#include <iostream> // std::cout
#include <string> // std::string
#include <array> // std::array

namespace tinyNet
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Default Ctor.
    Server::Server(std::uint16_t port)
        : port_{ port }
    {
#if defined(TINY_NET_WINDOWS)
        WSADATA wsaData{};
        auto const iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        checkValue(iResult, "WSAStartup failed");
#endif // defined(TINY_NET_WINDOWS)
        socketFd_ = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

#if defined(TINY_NET_LINUX)
        checkValue(socketFd_ == -1, "Failed creating socket()");
#else
        checkValue(socketFd_ == INVALID_SOCKET, "Failed creating socket()");

        int const onlyIPv6{ 0 };
        auto const set = setsockopt(socketFd_, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char const*>(&onlyIPv6), sizeof(onlyIPv6));
        checkValue(set, "Failed to enable IPv6 compatibility with IPv4");

#endif // defined(TINY_NET_LINUX)


        sockaddr_in6 serverInfo{};
        serverInfo.sin6_family = AF_INET6;
        serverInfo.sin6_addr = in6addr_any;
        serverInfo.sin6_port = htons(port_);

        auto const binded = bind(socketFd_, reinterpret_cast<sockaddr const*>(&serverInfo), sizeof(serverInfo));
        checkValue(binded, "Failed bind()");

        std::cout << "Server listening port: " << port_ << '\n';
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Default Dtor.
    Server::~Server() noexcept
    {
        std::cout << "Server destruction!\n";
#if defined(TINY_NET_LINUX)
        close(socketFd_);
#else
        closesocket(socketFd_);
        WSACleanup();
#endif // defined(TINY_NET_LINUX)
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Recieve Messages and print them.
    bool Server::recieveMessages() noexcept
    {
        std::array<char, 1024> buffer{};
        sockaddr_in6 clientInfo{};

#if defined(TINY_NET_LINUX)
        socklen_t size{ sizeof(clientInfo) };
        auto const recfrom = recvfrom(socketFd_, buffer.data(), buffer.size(), MSG_WAITALL, reinterpret_cast<sockaddr*>(&clientInfo), &size);
#else
        int size{ sizeof(clientInfo) };
        auto const recfrom = recvfrom(socketFd_, buffer.data(), int(buffer.size()), 0, reinterpret_cast<sockaddr*>(&clientInfo), &size);
#endif // defined(TINY_NET_LINUX)

        if (recfrom == -1)
        {
#if defined(TINY_NET_LINUX)
            std::cerr << "Failed recvfrom() " << strerror(errno) << "\n";
#else
            std::string error{};
            error.resize(256);
            auto const wsa_error = WSAGetLastError();
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, wsa_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error.data(), sizeof(error), nullptr);
            std::cerr << "Failed recvfrom() " << error << "\n";
#endif // defined(TINY_NET_LINUX)
            return false;
        }

        std::string const recieved { buffer.begin(), buffer.begin() + recfrom };
        if (recieved == "quit")
        {
            return false;
        }

        messages_.emplace(Message{ recieved });

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print Messages.
    void Server::pollMessages() noexcept
    {
        std::cout << "Messages:" << messages_.size() << '\n';
        while (!messages_.empty())
        {
            auto const& front = messages_.front();
            std::cout << "[\t" + front.data + "\t]\n";
            messages_.pop();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check Value.
    void Server::checkValue(int const value, std::string message)
    {
#if defined(TINY_NET_LINUX)
        std::string error{ strerror(errno) };
#else
        std::string error{};
        error.resize(256);
        auto const wsa_error = WSAGetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, wsa_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error.data(), sizeof(error), nullptr);
#endif // defined(TINY_NET_LINUX)

        message += " : " + error;

        if (value != 0)
        {
            throw std::invalid_argument(message + '\n');
        }
    }

} // namespace tinyNet
