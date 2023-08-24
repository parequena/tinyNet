#include <ServerHandler.hpp>

#include <algorithm>

namespace tinyNet
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set max clients
bool ServerHandler::addClient(std::string const& clientInfo) noexcept
{
   if(currentClients_ == maxClients_)
   {
      return false;
   }

   auto const found = std::find(clients_.begin(), clients_.end(), clientInfo) != clients_.end();
   if(found)
   {
      return false;
   }

   ++currentClients_;
   clients_.emplace_back(clientInfo);

   return true;
}
} // namespace tinyNet