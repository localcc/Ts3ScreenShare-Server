#include <iostream>
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#endif
#define ENET_IMPLEMENTATION
#include <enet.h>
#include <network/udp_server.h>
#include <cmath>
int64_t roundUp(int64_t numToRound, size_t multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = std::abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;

    if (numToRound < 0)
        return -(std::abs(numToRound) - remainder);
    else
        return numToRound + multiple - remainder;
}

void* enet_custom_malloc(size_t s) {

    const auto actualAl = roundUp(static_cast<int16_t>(s), 16);
    auto* outP = new uint8_t[actualAl];
    return reinterpret_cast<void*>(outP);
}

void enet_custom_free(void* mem) {
    if(mem == nullptr) return;
    auto* inP = reinterpret_cast<uint8_t*>(mem);
    if(inP == nullptr) return;
    delete[] inP;
}


int main() {
    /*
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
    WSAData data{};
    int32_t res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(res < 0) {
        return res;
    }
#endif*/
    /*
    ENetCallbacks callbacks = { malloc, free, abort, enet_packet_create, enet_packet_destroy };*/

    ENetCallbacks callbacks = {enet_custom_malloc, enet_custom_free, abort, enet_packet_create, enet_packet_destroy};
    enet_initialize_with_callbacks(ENET_VERSION, &callbacks);
    if(enet_initialize() != 0) {
        return -1;
    }
    udp_server server(30004);
    server.run();
}
