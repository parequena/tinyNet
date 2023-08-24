#ifndef TINYNET_CLIENT_HPP
#define TINYNET_CLIENT_HPP

#include "Socket.hpp"

#include <cstdint> // std::uint16_t

#if defined(TINY_NET_LINUX)
#include <netinet/in.h> // sockaddr_in6
#else
#include <ws2ipdef.h>   // sockaddr_in6
#endif                  // defined(TINY_NET_LINUX)

namespace tinyNet
{
struct Client
{
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Ctor.
   [[nodiscard]] explicit Client(std::string const &serverIp, std::uint16_t serverPort);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Send Message.
   [[nodiscard]] bool sendMessage() const noexcept;

private:
   // Sending socket.
   Socket socket_{};

   // Server info.
   sockaddr_in6 serverInfo_{};

   // Connected?
   bool connected_{ false };
};
} // namespace tinyNet
#endif /* TINYNET_CLIENT_HPP */
