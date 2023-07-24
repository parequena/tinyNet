#ifndef TINYNET_SERVER_HPP
#define TINYNET_SERVER_HPP

#include <cstdint> // std::uint16_t
#include <string> // std::string
#include <queue> // std::queue

#if defined(__CYGWIN__) || defined(__MSYS__)
#define TINY_NET_WINDOWS
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
    // Socket filde descryptor.
    int socketFd_{};

    // Listening port
    std::uint16_t port_{};

    // Messages.
    std::queue<Message> messages_{};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check Value.
    static void checkValue(int const value, std::string const& message);

};
} // namespace tinyNet

#endif /* TINYNET_SERVER_HPP */