#pragma once
#include <cstdint>

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#elif defined(__linux) || defined(__linux__) || defined(__gnu_linux__) || defined(linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#elif defined(__APPLE__)

#endif
#include <cstdint>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <network/connected_client.h>
#include <functional>
#include <tuple>
#include <enet.h>


class udp_server {
public:
    udp_server(int16_t port);

    void run();

    ~udp_server();
private:
    bool running;
    ENetHost* server;
    //std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> server;

    //std::unordered_map<sockaddr_in, std::unique_ptr<connected_client>> connectedClients;
    std::unordered_map<uint64_t, std::unique_ptr<connected_client>> connectedClients;


    void handle_login(ENetEvent event);
    void handle_frame(ENetEvent event);
    void handle_start_watch_stream(ENetEvent event);
    void handle_stop_watch_stream(ENetEvent event);
    void handle_start_stream(ENetEvent event);
    void handle_disconnect(ENetEvent event);

};
