#include <Server.hpp>

#include <iostream> // std::cout
#include <string>   // std::string

#if defined(TINY_NET_LINUX)
#include <netinet/ip.h> // sockaddr_in6, AF_INET6
#include <cstring>     // strerror
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

   Socket::checkValue(int(socket_.Bind(serverInfo)), "Failed to bind()");
   std::cout << "Server listening port: " << port_ << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recieve Messages and print them.
bool Server::recieveMessages() noexcept
{
   sockaddr_in6 clientInfo{};

   auto const recieved = socket_.RecieveMessage(clientInfo);
   if (recieved.empty())
   {
#if defined(TINY_NET_LINUX)
      std::cerr << "Failed recvfrom() " << strerror(errno) << "\n";
#else
      std::cerr << "Failed recvfrom() " << std::to_string(WSAGetLastError()) << "\n";
#endif // defined(TINY_NET_LINUX)
      return false;
   }

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
