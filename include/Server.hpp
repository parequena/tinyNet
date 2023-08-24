#ifndef TINYNET_SERVER_HPP
#define TINYNET_SERVER_HPP

#include "Socket.hpp"

#include <cstdint> // std::uint16_t
#include <queue>   // std::queue

namespace tinyNet
{
struct Server
{
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Ctor.
   [[nodiscard]] explicit Server(std::uint16_t port);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Recieve Messages.
   [[nodiscard]] bool recieveMessages() noexcept;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Print Messages.
   void pollMessages() noexcept;

private:
   // Listening socket.
   Socket socket_{};

   // Listening port.
   std::uint16_t port_{};

   // Messages.
   std::queue<std::string> messages_{};
};
} // namespace tinyNet
#endif /* TINYNET_SERVER_HPP */
