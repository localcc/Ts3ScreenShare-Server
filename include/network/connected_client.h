#pragma once

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#elif defined(__linux) || defined(__linux__) || defined(__gnu_linux__) || defined(linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#elif defined(__APPLE__)

#endif
#include <enet.h>
#include <memory>
#include <vector>

class connected_client {
public:
    connected_client(std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> peer);

    int32_t write(const uint8_t* data, int64_t size);

    void addWatchingMe(uint16_t id);
    void removeWatchingMe(uint16_t id);

    void addWatchedByMe(uint16_t id);
    void removeWatchedByMe(uint16_t id);

    const std::vector<uint16_t> getWatchingMe() const;
    const std::vector<uint16_t> getWatchedByMe() const;

    ~connected_client() = default;
private:
    std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> peer;

    std::vector<uint16_t> watching_me;
    std::vector<uint16_t> watched_by_me;
};
