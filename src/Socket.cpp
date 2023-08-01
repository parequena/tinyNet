#include <Socket.hpp>

#include <stdexcept> // std::invalid_argument

#if defined(TINY_NET_LINUX)
#include <netinet/ip.h> // socket, AF_INET6, SOCK_DGRAM, IPPROTO_UDP, IPPROTO_IPV6, IPV6_V6ONLY
#include <unistd.h>     // close
#include <string.h>     // strerror
#else
#include <ws2ipdef.h>   // IPV6_V6ONLY
#endif                  // defined(TINY_NET_LINUX)

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Socket::Socket()
{
#if defined(TINY_NET_WINDOWS)
   WSADATA wsaData{};
   checkValue(WSAStartup(MAKEWORD(2, 2), &wsaData), "WSAStartup failed");
#endif // defined(TINY_NET_WINDOWS)

   // Socket creation.
   fd_ = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

   // Enabling IPv4 compatibility.
   int const onlyIPv6{ 0 };
   auto const set
     = setsockopt(fd_, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char const*>(&onlyIPv6), sizeof(onlyIPv6));
   checkValue(set, "Failed to enable IPv6 compatibility with IPv4");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Dtor.
Socket::~Socket()
{
#if defined(TINY_NET_LINUX)
   close(fd_);
#else
   closesocket(fd_);
   WSACleanup();
#endif // defined(TINY_NET_LINUX)
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check Value.
void Socket::checkValue(int const value, std::string message)
{
#if defined(TINY_NET_LINUX)
   std::string error{ strerror(errno) };
#else
   std::string error{ std::to_string(WSAGetLastError()) };
#endif // defined(TINY_NET_LINUX)

   message += " : " + error;

   if (value != 0)
   {
      throw std::invalid_argument(message + '\n');
   }
}
} // namespace tinyNet
