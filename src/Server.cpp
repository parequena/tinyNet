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
// Initialize handler.
ServerHandler Server::handler_{};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default Ctor.
Server::Server(std::uint16_t port, callback_t const& callback_raw_string)
    : port_{ port }
{
   sockaddr_in6 serverInfo{};
   serverInfo.sin6_family = AF_INET6;
   serverInfo.sin6_addr = in6addr_any;
   serverInfo.sin6_port = htons(port_);

   Socket::checkValue(int(socket_.Bind(serverInfo)), "Failed to bind()");
   std::cout << "Server listening port: " << port_ << '\n';

   handler_.maxClients(1);
   if(callback_raw_string)
   {
      std::cout << "callback\n";
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recieve Messages and print them.
void Server::recieveMessages() const noexcept
{
   while(true)
   {
      sockaddr_in6 clientInfo{};
      auto const recieved = socket_.RecieveMessage(clientInfo);

      auto const type = recieved.type();

      if(type == Message_Type::Message_Type_CLIENT_CONN)
      {
         auto const clientIp = Socket::GetIp(clientInfo);
         auto const added = handler_.addClient(clientIp);
         
         Message msg{};
         auto const msg_type = added ? Message_Type::Message_Type_CLINET_CONN_ACK_ACC : Message_Type::Message_Type_CLINET_CONN_ACK_REJ;
         msg.set_type( msg_type );

         socket_.SendMessage(clientInfo, msg);
         continue;
      }

      if(type == Message_Type::Message_Type_RAW_STRING)
      {
         std::cout << "Recieved: " << recieved.data() << '\n';
         continue;
      }

      return;
   }
}
} // namespace tinyNet
