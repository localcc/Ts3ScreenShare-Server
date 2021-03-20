#include <iostream>
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#endif
#define ENET_IMPLEMENTATION
#include <enet.h>
#include <network/udp_server.h>

int main() {
    /*
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
    WSAData data{};
    int32_t res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(res < 0) {
        return res;
    }
#endif*/
    if(enet_initialize() != 0) {
        return -1;
    }
    udp_server server(30004);
    server.run();
}
