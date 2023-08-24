#ifndef TINYNET_SOCKET_HPP
#define TINYNET_SOCKET_HPP

#include <string> // std::string

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include <Message.pb.h> // Include Message from Protobuf
#pragma GCC diagnostic pop

#if defined(__CYGWIN__) || defined(__MSYS__) || defined(_WIN64)
#define TINY_NET_WINDOWS
#include <winsock2.h>
#else // defined(__linux__) && defined(__APPLE__)
#define TINY_NET_LINUX
#endif

struct sockaddr_in6;

namespace tinyNet
{
struct Socket
{
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Ctor.
   [[nodiscard]] explicit Socket();

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Dtor.
   ~Socket();

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Bind socket.
   bool Bind(sockaddr_in6 const& toBind) const noexcept;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Send Message.
   bool SendMessage(sockaddr_in6 const& destination, tinyNet::Message const& message) const noexcept;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Recieve Message.
   Message RecieveMessage(sockaddr_in6 &from) const;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Check Value.
   static void checkValue(int const value, std::string message);

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Get Ip.
   [[nodiscard]] static std::string GetIp(sockaddr_in6 const& info);

   constexpr Socket(Socket const&) = delete;
   constexpr Socket& operator=(Socket const&) = delete;
   constexpr Socket(Socket const&&) = delete;
   constexpr Socket& operator=(Socket const&&) = delete;

private:
   // Socket file descriptor.
#if defined(TINY_NET_LINUX)
   int fd_{};
#else
   SOCKET fd_{};
#endif

   static constexpr std::size_t bufferSize_{ 1024 };

};
} // namespace tinyNet
#endif /* TINYNET_SOCKET_HPP */
