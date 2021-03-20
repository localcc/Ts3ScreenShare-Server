#include <network/udp_server.h>
#include <iostream>
#include <network/NetworkHelper.h>

udp_server::udp_server(int16_t port) :
    connectedClients() {
    this->sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(this->sock_fd < 0) {
        throw std::runtime_error("Failed to create socket!");
    }

    sockaddr_in addr{};
    addr.sin_port = port;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->sock_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        throw std::runtime_error("Failed to bind to port!");
    }
}

void udp_server::run() {
    this->running = true;

    sockaddr_in addr{};
    uint32_t addrSz = sizeof(addr);

    auto* szBuf = new uint8_t[4];
    while(this->running) {
        std::fill_n(szBuf, 4, 0);

        recvfrom(this->sock_fd, szBuf, 4, MSG_PEEK, reinterpret_cast<sockaddr*>(&addr), &addrSz);

        uint32_t bufSize = NetworkHelper::ConvertU32(szBuf, 0);
        auto* recvBuf = new uint8_t[bufSize + 4];

        recvfrom(this->sock_fd, recvBuf, bufSize + 4, 0, reinterpret_cast<sockaddr*>(&addr), &addrSz);

        const auto search = this->callbacks.find(recvBuf[5]);
        if(search != this->callbacks.end())  {
            search->second(this, recvBuf, addr, addrSz);
        }

        delete[] recvBuf;
    }
}

void udp_server::handle_login(udp_server* server, const uint8_t* data, sockaddr_in sockaddr, int32_t sockaddrSize) {
    const auto clientId = NetworkHelper::ConvertU16(data, 6);
    const auto search = server->connectedClients.find(clientUid);
    if(search != server->connectedClients.end()) return;

    sockaddr_in idAddr{};
    std::copy(&sockaddr, &sockaddr + sizeof(sockaddr_in), &idAddr);

    std::unique_ptr<sockaddr_in> copy;
    std::copy(&sockaddr, &sockaddr + sizeof(sockaddr_in), copy.get());

    std::unique_ptr<connected_client> client = std::make_unique<connected_client>(server->sock_fd, std::move(copy), sockaddrSize);

    server->connectedClients.emplace(idAddr, std::move(client));
}

void udp_server::handle_frame(udp_server* server, const uint8_t* data, sockaddr_in sockaddr, int32_t sockaddrSize) {
    const auto clientId = NetworkHelper::ConvertU16(data, 6);

}

void udp_server::handle_start_watch_stream(udp_server* server, const uint8_t* data, sockaddr_in sockaddr, int32_t sockaddrSize) {

}

void udp_server::handle_stop_watch_stream(udp_server* server, const uint8_t* data, sockaddr_in sockaddr, int32_t sockaddrSize) {

}

void udp_server::handle_disconnect(udp_server* server, const uint8_t* data, sockaddr_in sockaddr, int32_t sockaddrSize) {

}

udp_server::~udp_server() {
    shutdown(this->sock_fd, SHUT_RDWR);
}
