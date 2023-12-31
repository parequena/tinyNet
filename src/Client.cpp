#include <Client.hpp>

#include <iostream> // std::cin

#if defined(TINY_NET_LINUX)
#include <cstring>    // strerror
#include <arpa/inet.h> // inet_pton
#else
#include <ws2tcpip.h>  // inet_pton
// Link with sockets lib.
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Client::Client(std::string const &serverIp, std::uint16_t serverPort)
{
   serverInfo_.sin6_family = AF_INET6;
   serverInfo_.sin6_port = htons(serverPort);

   // InetPton(AF_INET6, cfg_->targetIp.data(), &addrTo.sin6_addr);
   auto const inet_v6 = inet_pton(AF_INET6, serverIp.data(), &serverInfo_.sin6_addr);
   if (inet_v6 != 1)
   {
      auto const mappedIP4{ "::ffff:" + serverIp };
      auto const inet_v4 = inet_pton(AF_INET6, mappedIP4.data(), &serverInfo_.sin6_addr);
      Socket::checkValue(int(inet_v4 != 1), "Server IP neither IPv4 or IPv6.");
   }

   Message connection{};
   connection.set_type(Message_Type::Message_Type_CLIENT_CONN);
   connected_ = socket_.SendMessage(serverInfo_, connection);
   auto const& serverConnAck = socket_.RecieveMessage(serverInfo_);
   if(serverConnAck.type() == Message_Type::Message_Type_CLINET_CONN_ACK_REJ)
   {
      throw std::domain_error( "Connection refused by server: " + serverIp + ":" + std::to_string(serverPort) );
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Send Message.
bool Client::sendMessage() const noexcept
{
   std::string buffer{};
   while (std::getline(std::cin, buffer))
   {
      if (buffer == "exit")
      {
         return false;
      }

      Message msg{};
      msg.set_type(Message_Type::Message_Type_RAW_STRING);
      msg.set_data(buffer);

      if (!socket_.SendMessage(serverInfo_, msg))
      {
#if defined(TINY_NET_LINUX)
         std::cerr << "Failed sendto() " << strerror(errno) << "\n";
#else
         std::cerr << "Failed sendto() " << std::to_string(WSAGetLastError()) << "\n";
#endif // defined(TINY_NET_LINUX)
         return false;
      }
   }

   return true;
}
} // namespace tinyNet
