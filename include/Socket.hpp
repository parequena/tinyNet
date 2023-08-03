#ifndef TINYNET_SOCKET_HPP
#define TINYNET_SOCKET_HPP

#include <string> // std::string

#if defined(__CYGWIN__) || defined(__MSYS__) || defined(_WIN64)
#define TINY_NET_WINDOWS
#include <winsock2.h>
#else // defined(__linux__) && defined(__APPLE__)
#define TINY_NET_LINUX
#endif

namespace tinyNet
{
struct Message
{
   std::string data{};
};

struct Socket
{
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Ctor.
   [[nodiscard]] explicit Socket();

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Default Dtor.
   ~Socket();

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Getter.
   [[nodiscard]] constexpr auto FileDescriptor() const noexcept { return fd_; }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Check Value.
   static void checkValue(int const value, std::string message);

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
};
} // namespace tinyNet
#endif /* TINYNET_SOCKET_HPP */
