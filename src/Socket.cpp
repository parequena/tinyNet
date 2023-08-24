#include <Socket.hpp>

#include <stdexcept> // std::invalid_argument
#include <array>    // std::array

#if defined(TINY_NET_LINUX)
#include <netinet/ip.h> // socket, AF_INET6, SOCK_DGRAM, IPPROTO_UDP, IPPROTO_IPV6, IPV6_V6ONLY
#include <unistd.h>     // close
#include <cstring>      // strerror
#include <arpa/inet.h>  // inet_ntop
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
// Bind socket.
bool Socket::Bind(sockaddr_in6 const& toBind) const noexcept
{
   auto const binded = bind(fd_, reinterpret_cast<sockaddr const*>(&toBind), sizeof(toBind));
   return binded != 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Send Message.
bool Socket::SendMessage(sockaddr_in6 const& destination, tinyNet::Message const& message) const noexcept
{
   std::array<std::uint8_t, bufferSize_> data{};
   message.SerializeToArray(&data, data.size());

#if defined(TINY_NET_LINUX)
   auto const sent = sendto(fd_, data.data(), data.size(), 0, reinterpret_cast<sockaddr const*>(&destination), sizeof(destination));
#else
   auto const sent = sendto(fd_, data.data(), int(data.size()), 0, reinterpret_cast<sockaddr const*>(&destination), sizeof(destination));
#endif // defined(TINY_NET_LINUX)

   return sent != 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recieve Message.
Message Socket::RecieveMessage(sockaddr_in6 &from) const
{
   std::array<char, bufferSize_> buffer{};

#if defined(TINY_NET_LINUX)
   socklen_t size{ sizeof(from) };
   std::size_t const buffer_size{ buffer.size() };
#else
   int size{ sizeof(clientInfo) };
   int const buffer_size{ buffer.size() };
#endif // defined(TINY_NET_LINUX)

   auto const recfrom = recvfrom(fd_, buffer.data(), buffer_size, 0, reinterpret_cast<sockaddr*>(&from), &size);
   Socket::checkValue(int(recfrom == -1), "Failed recvfrom()");

   Message ret{};
   ret.ParseFromArray(&buffer, buffer.size());

   return ret;
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
   std::string const error{ strerror(errno) };
#else
   std::string error{ std::to_string(WSAGetLastError()) };
#endif // defined(TINY_NET_LINUX)

   message += " : " + error;

   if (value != 0)
   {
      throw std::invalid_argument(message + '\n');
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get Ip.
std::string Socket::GetIp(sockaddr_in6 const& info)
{
   std::array<char, INET6_ADDRSTRLEN> str{};
   auto const ip = inet_ntop(AF_INET6, &info, str.data(), INET6_ADDRSTRLEN);

   return ip == nullptr ? "" : ip;
}
} // namespace tinyNet
