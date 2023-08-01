#include <Client.hpp>

#include <arpa/inet.h> // inet_pton
#include <iostream> // std::cin

#if defined(TINY_NET_LINUX)
#include <string.h> // strerror
#endif

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Client::Client(std::string const serverIp, std::uint16_t serverPort)
{
   serverInfo_.sin6_family = AF_INET6;
   serverInfo_.sin6_port = htons(serverPort);

   auto const inet_v6 = inet_pton(AF_INET6, serverIp.data(), &serverInfo_.sin6_addr);
   if(inet_v6 != 1)
   {
      auto const inet_v4 = inet_pton(AF_INET, serverIp.data(), &serverInfo_.sin6_addr);
      serverInfo_.sin6_family = AF_INET;
      Socket::checkValue(inet_v4 != 1, "Server IP neither IPv4 or IPv6.");
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Send Message.
bool Client::sendMessage() const noexcept
{
   std::string buffer{};
   while (std::getline(std::cin, buffer))
   {
      if(buffer == "exit") { return false; }

      auto const sent = sendto(socket_.FileDescriptor(), buffer.data(), buffer.size(), 0, reinterpret_cast<sockaddr const*>(&serverInfo_), sizeof(serverInfo_));
      if (sent == -1)
      {
#if defined(TINY_NET_LINUX)
         std::cerr << "Failed sendto() " << strerror(errno) << "\n";
#else
         std::string error{};
         error.resize(256);
         auto const wsa_error = WSAGetLastError();
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, wsa_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error.data(), sizeof(error), nullptr);
         std::cerr << "Failed sendto() " << error << "\n";
#endif // defined(TINY_NET_LINUX)
         return false;
      }
   }
   
   // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
   return true;
}
} // namespace tinyNet