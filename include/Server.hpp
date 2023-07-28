#ifndef TINYNET_SERVER_HPP
#define TINYNET_SERVER_HPP

#include <cstdint> // std::uint16_t
#include <string> // std::string
#include <queue> // std::queue

#if defined(__CYGWIN__) || defined(__MSYS__) || defined(_WIN64)
#define TINY_NET_WINDOWS
#include <winsock2.h>
#elif defined(__linux__)
#define TINY_NET_LINUX
#else
#endif

namespace tinyNet
{

struct Message
{
    std::string data{};
};

struct Server
{
    using message_t = Message;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Default Ctor.
    [[nodiscard]] explicit Server(std::uint16_t port);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Default Dtor.
    ~Server() noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Recieve Messages.
    [[nodiscard]] bool recieveMessages() noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Print Messages.
    void pollMessages() noexcept;

private:
    // Listening port
    std::uint16_t port_{};
    
    // Padding!
    [[maybe_unused]] std::uint16_t mp_error_on_use_padding_solution{};

    // Socket filde descryptor.
#if defined(TINY_NET_LINUX)
    int socketFd_{};
#else
    SOCKET socketFd_{};
#endif

    // Messages.
    std::queue<Message> messages_{};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check Value.
    static void checkValue(int const value, std::string message);

};
} // namespace tinyNet

#endif /* TINYNET_SERVER_HPP */
