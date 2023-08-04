#include <Server.hpp>

#include <iostream> // std::cout
#include <string>   // std::string
#include <array>    // std::array

#if defined(TINY_NET_LINUX)
#include <netinet/ip.h> // sockaddr_in6, AF_INET6
#include <string.h>     // strerror
#else
#include <ws2ipdef.h>
// Link with sockets lib.
#pragma comment(lib, "Ws2_32.lib")
#endif // defined(TINY_NET_LINUX)

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Server::Server(std::uint16_t port)
    : port_{ port }
{
   sockaddr_in6 serverInfo{};
   serverInfo.sin6_family = AF_INET6;
   serverInfo.sin6_addr = in6addr_any;
   serverInfo.sin6_port = htons(port_);

   auto const binded
     = bind(socket_.FileDescriptor(), reinterpret_cast<sockaddr const*>(&serverInfo), sizeof(serverInfo));
   Socket::checkValue(binded, "Failed bind()");

   std::cout << "Server listening port: " << port_ << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recieve Messages and print them.
bool Server::recieveMessages() noexcept
{
   std::array<char, 1024> buffer{};
   sockaddr_in6 clientInfo{};

#if defined(TINY_NET_LINUX)
   socklen_t size{ sizeof(clientInfo) };
   std::size_t const buffer_size{ buffer.size() };
#else
   int size{ sizeof(clientInfo) };
   int const buffer_size{ buffer.size() };
#endif // defined(TINY_NET_LINUX)

   auto const recfrom = recvfrom(socket_.FileDescriptor(), buffer.data(), buffer_size, 0, reinterpret_cast<sockaddr*>(&clientInfo), &size);
   if (recfrom == -1)
   {
#if defined(TINY_NET_LINUX)
      std::cerr << "Failed recvfrom() " << strerror(errno) << "\n";
#else
      std::cerr << "Failed recvfrom() " << std::to_string(WSAGetLastError()) << "\n";
#endif // defined(TINY_NET_LINUX)
      return false;
   }

   std::string const recieved{ buffer.begin(), buffer.begin() + recfrom };
   if (recieved == "quit")
   {
      return false;
   }

   messages_.emplace(Message{ recieved });

   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
} // namespace tinyNet
