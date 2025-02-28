# tinyNet
---

Tiny Networking examples done on streaming via [Twitch](www.twitch.tv/theApoCa) (In Spanish) and It's saved on [Youtube](https://www.youtube.com/playlist?list=PLBEviA0cKSijxLaqOVXHsZAxSwb9yYml4)<br/>
Inspired by this [theCherno video](https://www.youtube.com/watch?v=jS9rBienEFQ), I wanted to create the following videos/features.
 * [x] Create Server UDP (Linux/Windows).
 * [x] Create Client UDP (Linux/Windows).
    * [x] Move all Common code into a new `struct/class`
    * [ ] Client maybe will create a [jthread](https://en.cppreference.com/w/cpp/thread/jthread) to wait messages from server.
 * [ ] Change `std::string` by [protobuf](https://github.com/protocolbuffers/protobuf) for message interchange.
 * [ ] Compress/decompress messages (with some compression algorithm, just for fun).
 * [ ] Encrypt messages with something simple (just for fun).

I'm following and trying to create something simpler of [Valve's GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets).

## Building
### Dependencies
* CMake 3.10 or later
* A build tool like Ninja, GNU Make or Visual Studio
* A C++23-compliant compiler, such as:
  * GCC 13 or later
  * Clang 16 or later
  * Visual Studio 2022 or later
* Vcpkg
  * build-essential
  * tar
  * curl
  * zip
  * unzip

#### Using CMake (preferred):

##### CMake presets:
* gcc:
    * gcc_debug
    * gcc_release
* clang:
    * clang_debug
    * clang_release
* Windows MVSC (Visual Studio 2022):
    * wind_debug
    * wind_release
* Windows Clang (Visual Studio 2022):
    * wind_clang_debug
    * wind_clang_release

Let's see an example with `gcc_release` as preset, but you can choose another from this list.
```
$ git clone --recurse-submodules https://github.com/parequena/tinyNet
$ ./libs/vcpkg/bootstrap-vcpkg.sh
$ mkdir build
$ cd build
$ cmake .. --preset gcc_release
$ cd gcc_release
$ cmake --build . -j
```

# Get in Touch
If you want to get in touch with me, here are my social networks
- [Twitter](https://x.com/conPdePABLO)
- [BlueSky](https://bsky.app/profile/theapoca.bsky.social)
- [Youtube](https://www.youtube.com/@conpdepab)
- [Instagram](https://www.instagram.com/conpdepab/)
- [LinkedIn](https://www.linkedin.com/in/parequena/)
