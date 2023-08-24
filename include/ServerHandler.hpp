#ifndef TINYNET_SERVERHANDLER_HPP
#define TINYNET_SERVERHANDLER_HPP

#include <cstdint> // std::uint8_t
#include <string> // std::string
#include <vector> // std::vector

namespace tinyNet
{
struct ServerHandler
{
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Set max clients
   constexpr void maxClients(std::uint8_t const maxClients) noexcept { maxClients_ = maxClients; }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Set max clients
   [[nodiscard]] bool addClient(std::string const& clientInfo) noexcept;

private:
   std::vector<std::string> clients_{};

   std::uint8_t currentClients_{};
   std::uint8_t maxClients_{};
};
} // namespace tinyNet
#endif /* TINYNET_SERVERHANDLER_HPP */
