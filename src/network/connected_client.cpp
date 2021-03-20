#include <network/connected_client.h>

connected_client::connected_client(int32_t server_sock_fd, std::unique_ptr<sockaddr_in> srcAddr, int32_t addrSz) :
    server_sock_fd{server_sock_fd}, addr(std::move(srcAddr)), addrSz{addrSz} {
}

int32_t connected_client::write(const uint8_t* data, int64_t size) {
    return sendto(this->server_sock_fd, data, size, 0, reinterpret_cast<sockaddr*>(&this->addr), size);
}
