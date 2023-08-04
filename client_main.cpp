#include <iostream> // std::cerr
#include <span>     // std::span
#include <vector>   // std::vector
#include <ranges>   // std::ranges::subrange

#include <Server.hpp>
#include <Client.hpp>

static int better_main(std::span<std::string> const args)
{
   if (args.size() < 2)
   {
      std::cerr << "Failed with arguments, try executing:\n./executable <server_address> <server_port>\n";
      return EXIT_FAILURE;
   }

   int const original_port{ std::stoi(args[1]) };
   int const uint16_max = 0xffff; // std::numeric_limits<std::uint16_t>::max();
   if (original_port < 0 || original_port > uint16_max)
   {
      std::cerr << "ERROR: Port number invalid, found: " << std::to_string(original_port)
                << ", must be in range: [ 0 - " << std::to_string(uint16_max) << "].\n";
      return EXIT_FAILURE;
   }

   std::uint16_t const port = static_cast<std::uint16_t>(original_port);
   tinyNet::Client client{ args[0], port };

   while (client.sendMessage())
      ;

   return EXIT_SUCCESS;
}

int main(int const argc, char const* const* const argv)
{
   auto result = std::ranges::subrange(&argv[1], &argv[argc]);
   std::vector<std::string> args{ result.begin(), result.end() };
   
   return better_main(args);
}
