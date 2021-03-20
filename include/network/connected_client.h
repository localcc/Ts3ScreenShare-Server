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
#include <memory>

class connected_client {
public:
    connected_client(int32_t server_sock_fd, std::unique_ptr<sockaddr_in> srcAddr, int32_t addrSz);

    int32_t write(const uint8_t* data, int64_t size);

    ~connected_client() = default;
private:


    int32_t server_sock_fd;
    std::unique_ptr<sockaddr_in> addr;
    int32_t addrSz;
};
