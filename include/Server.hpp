#ifndef TINYNET_SERVER_HPP
#define TINYNET_SERVER_HPP

#include "Socket.hpp"
#include "ServerHandler.hpp"

#include <cstdint> // std::uint16_t
#include <functional> // std::function
#include <unordered_map> // std::unordered_map

namespace tinyNet
{
struct Server
{
   using callback_t = std::function<void()>;
   using callMap_t = std::unordered_map<Message_Type, callback_t>;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Ctor.
   [[nodiscard]] explicit Server(std::uint16_t port, callback_t const& callback_raw_string = nullptr);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Recieve Messages.
   void recieveMessages() const noexcept;

private:
   // Listening socket.
   Socket socket_{};

   // Listening port.
   std::uint16_t port_{};

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Server Handler.
   static ServerHandler handler_;
};
} // namespace tinyNet
#endif /* TINYNET_SERVER_HPP */
